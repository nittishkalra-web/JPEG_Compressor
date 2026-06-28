import { ImageUp, SlidersHorizontal } from 'lucide-react';
import { useState } from 'react';

const filters = ['none', 'grayscale', 'warm', 'cool', 'vintage', 'contrast', 'saturation'];
const modes = ['baseline', 'optimized', 'compare'];

export default function UploadPanel({ onCompress, loading }) {
  const [file, setFile] = useState(null);
  const [quality, setQuality] = useState(50);
  const [filter, setFilter] = useState('warm');
  const [mode, setMode] = useState('compare');

  function submit(event) {
    event.preventDefault();
    if (file) onCompress({ file, quality, filter, mode });
  }

  return (
    <form className="panel upload-panel" onSubmit={submit}>
      <div className="panel-title">
        <ImageUp size={20} />
        <h2>Input</h2>
      </div>

      <label className="file-drop">
        <input type="file" accept="image/*" onChange={(event) => setFile(event.target.files?.[0] || null)} />
        <span>{file ? file.name : 'Choose an image'}</span>
      </label>

      <label className="control-row">
        <span>Quality</span>
        <strong>{quality}</strong>
      </label>
      <input className="slider" type="range" min="1" max="100" value={quality} onChange={(event) => setQuality(event.target.value)} />

      <div className="two-controls">
        <label>
          <span>YCbCr filter</span>
          <select value={filter} onChange={(event) => setFilter(event.target.value)}>
            {filters.map((item) => <option key={item} value={item}>{item}</option>)}
          </select>
        </label>
        <label>
          <span>Mode</span>
          <select value={mode} onChange={(event) => setMode(event.target.value)}>
            {modes.map((item) => <option key={item} value={item}>{item}</option>)}
          </select>
        </label>
      </div>

      <button className="primary-button" disabled={!file || loading} type="submit">
        <SlidersHorizontal size={18} />
        {loading ? 'Running pipeline' : 'Compress'}
      </button>
    </form>
  );
}
