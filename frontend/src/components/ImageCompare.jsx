import { Download } from 'lucide-react';

export default function ImageCompare({ originalUrl, compressedUrl }) {
  async function downloadCompressed() {
    if (!compressedUrl) return;
    const response = await fetch(compressedUrl);
    const blob = await response.blob();
    const objectUrl = URL.createObjectURL(blob);
    const link = document.createElement('a');
    const fileName = compressedUrl.split('/').pop() || 'compressed-image.bmp';
    link.href = objectUrl;
    link.download = fileName;
    document.body.appendChild(link);
    link.click();
    link.remove();
    URL.revokeObjectURL(objectUrl);
  }

  return (
    <section className="panel image-panel">
      <div className="panel-title image-panel-title">
        <h2>Image Compare</h2>
        <button className="download-button" type="button" disabled={!compressedUrl} onClick={downloadCompressed}>
          <Download size={17} />
          Download image
        </button>
      </div>
      <div className="image-grid">
        <figure>
          <div className="image-frame">{originalUrl ? <img src={originalUrl} alt="Original upload" /> : <span>Original</span>}</div>
          <figcaption>Original</figcaption>
        </figure>
        <figure>
          <div className="image-frame">{compressedUrl ? <img src={compressedUrl} alt="Compressed reconstruction" /> : <span>Compressed</span>}</div>
          <figcaption>Reconstructed preview</figcaption>
        </figure>
      </div>
    </section>
  );
}
