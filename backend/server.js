const express = require('express');
const cors = require('cors');
const multer = require('multer');
const sharp = require('sharp');
const fs = require('fs');
const path = require('path');
const { execFile } = require('child_process');

const app = express();
const PORT = 5000;
const uploadDir = path.join(__dirname, 'uploads');
const outputDir = path.join(__dirname, 'outputs');
const supportedFilters = new Set(['none', 'grayscale', 'warm', 'cool', 'vintage', 'contrast', 'saturation']);
const supportedModes = new Set(['baseline', 'optimized', 'compare']);

fs.mkdirSync(uploadDir, { recursive: true });
fs.mkdirSync(outputDir, { recursive: true });

app.use(cors());
app.use('/outputs', express.static(outputDir));

const upload = multer({
  dest: uploadDir,
  limits: { fileSize: 15 * 1024 * 1024 },
  fileFilter: (_req, file, cb) => {
    if (file.mimetype.startsWith('image/')) cb(null, true);
    else cb(new Error('Only image uploads are supported.'));
  }
});

function compressorCandidates() {
  const executableName = process.platform === 'win32' ? 'jpeg_compressor.exe' : 'jpeg_compressor';
  const buildDir = path.resolve(__dirname, '../cpp-compressor/build');
  const candidates = process.platform === 'win32'
    ? [
        path.join(buildDir, 'Release', executableName),
        path.join(buildDir, executableName),
        path.join(buildDir, 'Debug', executableName),
        path.join(buildDir, 'RelWithDebInfo', executableName),
        path.join(buildDir, 'MinSizeRel', executableName)
      ]
    : [
        path.join(buildDir, executableName),
        path.join(buildDir, 'Release', executableName),
        path.join(buildDir, 'Debug', executableName),
        path.join(buildDir, 'RelWithDebInfo', executableName),
        path.join(buildDir, 'MinSizeRel', executableName)
      ];

  if (process.env.JPEG_COMPRESSOR_EXECUTABLE) {
    return [path.resolve(process.env.JPEG_COMPRESSOR_EXECUTABLE), ...candidates];
  }
  return candidates;
}

function compressorPath() {
  const candidates = compressorCandidates();
  const existing = candidates.find((candidate) => fs.existsSync(candidate));
  return existing || candidates[0];
}

function missingExecutableMessage() {
  const candidates = compressorCandidates().map((candidate) => `  - ${candidate}`).join('\n');
  if (process.platform === 'win32') {
    return `C++ executable not found. Build it first with: cd cpp-compressor && cmake -S . -B build && cmake --build build --config Release\nChecked:\n${candidates}`;
  }
  return `C++ executable not found. Build it first with: cd cpp-compressor && cmake -S . -B build && cmake --build build\nChecked:\n${candidates}`;
}

function runCompressor(args) {
  return new Promise((resolve, reject) => {
    execFile(compressorPath(), args, { windowsHide: true }, (error, stdout, stderr) => {
      if (error) {
        reject(new Error(`${stderr || error.message}\n${stdout || ''}`));
      } else {
        resolve();
      }
    });
  });
}

function writeBmp(filePath, width, height, pixels, channels) {
  const rowStride = Math.ceil((width * 3) / 4) * 4;
  const fileSize = 54 + rowStride * height;
  const buffer = Buffer.alloc(fileSize);
  buffer.write('BM', 0);
  buffer.writeUInt32LE(fileSize, 2);
  buffer.writeUInt32LE(54, 10);
  buffer.writeUInt32LE(40, 14);
  buffer.writeInt32LE(width, 18);
  buffer.writeInt32LE(height, 22);
  buffer.writeUInt16LE(1, 26);
  buffer.writeUInt16LE(24, 28);
  buffer.writeUInt32LE(rowStride * height, 34);
  buffer.writeInt32LE(2835, 38);
  buffer.writeInt32LE(2835, 42);

  for (let y = 0; y < height; y += 1) {
    const srcY = height - 1 - y;
    const dstRow = 54 + y * rowStride;
    for (let x = 0; x < width; x += 1) {
      const src = (srcY * width + x) * channels;
      const dst = dstRow + x * 3;
      buffer[dst] = pixels[src + 2];
      buffer[dst + 1] = pixels[src + 1];
      buffer[dst + 2] = pixels[src];
    }
  }
  fs.writeFileSync(filePath, buffer);
}

app.post('/api/compress', upload.single('image'), async (req, res) => {
  const uploaded = req.file;
  if (!uploaded) return res.status(400).json({ error: 'No image file uploaded.' });

  const requestedQuality = Number.parseInt(req.body.quality, 10);
  const quality = Number.isFinite(requestedQuality) ? Math.max(1, Math.min(100, requestedQuality)) : 50;
  const filter = req.body.filter || 'none';
  const mode = req.body.mode || 'optimized';
  const id = `${Date.now()}-${Math.round(Math.random() * 1e9)}`;
  const inputBmp = path.join(uploadDir, `${id}-input.bmp`);
  const reconstructedPng = path.join(outputDir, `${id}-reconstructed.png`);
  const statsPath = path.join(outputDir, `${id}-stats.json`);

  try {
    if (!supportedFilters.has(filter)) {
      return res.status(400).json({ error: `Unsupported filter: ${filter}` });
    }
    if (!supportedModes.has(mode)) {
      return res.status(400).json({ error: `Unsupported mode: ${mode}` });
    }
    if (!fs.existsSync(compressorPath())) {
      return res.status(500).json({
        error: missingExecutableMessage(),
        expectedPath: compressorPath()
      });
    }

    const { data, info } = await sharp(uploaded.path)
      .rotate()
      .removeAlpha()
      .toColourspace('srgb')
      .raw()
      .toBuffer({ resolveWithObject: true });
    writeBmp(inputBmp, info.width, info.height, data, info.channels);
    await runCompressor([
      inputBmp,
      reconstructedPng,
      statsPath,
      '--quality', String(quality),
      '--filter', filter,
      '--mode', mode
    ]);
    const stats = JSON.parse(fs.readFileSync(statsPath, 'utf8'));
    stats.originalSizeBytes = uploaded.size;
    if (stats.estimatedCompressedBytes) {
      stats.compressionRatio = Number((uploaded.size / stats.estimatedCompressedBytes).toFixed(2));
    }
    res.json({
      imageUrl: `http://localhost:${PORT}/outputs/${path.basename(reconstructedPng)}`,
      stats
    });
  } catch (error) {
    const message = error.message.includes('unsupported image format')
      ? 'Unsupported image format. Please try a JPG, PNG, BMP, GIF, or WebP image.'
      : error.message;
    res.status(500).json({ error: message });
  } finally {
    fs.rm(uploaded.path, { force: true }, () => {});
    fs.rm(inputBmp, { force: true }, () => {});
  }
});

app.listen(PORT, () => {
  console.log(`JPEG Compressor backend running at http://localhost:${PORT}`);
});
