import { useEffect, useState } from 'react';
import UploadPanel from './components/UploadPanel.jsx';
import ImageCompare from './components/ImageCompare.jsx';
import StatsPanel from './components/StatsPanel.jsx';
import PerformancePanel from './components/PerformancePanel.jsx';
import BlockVisualizer from './components/BlockVisualizer.jsx';
import MemoryLayoutExplanation from './components/MemoryLayoutExplanation.jsx';

export default function App() {
  const [originalUrl, setOriginalUrl] = useState('');
  const [result, setResult] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  useEffect(() => {
    return () => {
      if (originalUrl) URL.revokeObjectURL(originalUrl);
    };
  }, [originalUrl]);

  async function handleCompress({ file, quality, filter, mode }) {
    setError('');
    setResult(null);
    if (originalUrl) URL.revokeObjectURL(originalUrl);
    setOriginalUrl(URL.createObjectURL(file));
    setLoading(true);
    const formData = new FormData();
    formData.append('image', file);
    formData.append('quality', quality);
    formData.append('filter', filter);
    formData.append('mode', mode);

    try {
      const response = await fetch('http://localhost:5000/api/compress', {
        method: 'POST',
        body: formData
      });
      const data = await response.json();
      if (!response.ok) throw new Error(data.error || 'Compression failed.');
      setResult(data);
    } catch (err) {
      setError(err.message);
    } finally {
      setLoading(false);
    }
  }

  return (
    <main className="app-shell">
      <section className="top-bar">
        <div>
          <p className="eyebrow">C++17 + React + Node.js</p>
          <h1>JPEG Compressor</h1>
        </div>
        <div className="status-pill">{loading ? 'Compressing' : 'Ready'}</div>
      </section>

      <section className="workspace">
        <UploadPanel onCompress={handleCompress} loading={loading} />
        <ImageCompare originalUrl={originalUrl} compressedUrl={result?.imageUrl} />
      </section>

      {error && <div className="error-box">{error}</div>}

      <section className="dashboard">
        <StatsPanel stats={result?.stats} />
        <PerformancePanel stats={result?.stats} />
      </section>

      <BlockVisualizer block={result?.stats?.visualBlock} />
      <MemoryLayoutExplanation />
    </main>
  );
}
