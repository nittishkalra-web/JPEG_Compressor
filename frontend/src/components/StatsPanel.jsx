import { BarChart3 } from 'lucide-react';

function numberValue(value, digits = 2, suffix = '') {
  return Number.isFinite(value) ? `${value.toFixed(digits)}${suffix}` : '-';
}

function bytes(value) {
  if (!value) return '0 B';
  if (value > 1024 * 1024) return `${(value / 1024 / 1024).toFixed(2)} MB`;
  if (value > 1024) return `${(value / 1024).toFixed(1)} KB`;
  return `${value} B`;
}

export default function StatsPanel({ stats }) {
  const rows = [
    ['Compression ratio', numberValue(stats?.compressionRatio, 2, 'x')],
    ['Original size', bytes(stats?.originalSizeBytes)],
    ['Estimated size', bytes(stats?.estimatedCompressedBytes)],
    ['PSNR', numberValue(stats?.psnr, 2, ' dB')],
    ['MSE', numberValue(stats?.mse)],
    ['8x8 blocks', stats?.blockCount ?? '-']
  ];

  return (
    <section className="panel">
      <div className="panel-title">
        <BarChart3 size={20} />
        <h2>Compression Stats</h2>
      </div>
      <div className="metric-list">
        {rows.map(([label, value]) => (
          <div className="metric" key={label}>
            <span>{label}</span>
            <strong>{value}</strong>
          </div>
        ))}
      </div>
    </section>
  );
}
