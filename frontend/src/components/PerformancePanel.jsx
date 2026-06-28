import { Gauge } from 'lucide-react';

function numberValue(value, digits = 2, suffix = '') {
  return Number.isFinite(value) ? `${value.toFixed(digits)}${suffix}` : '-';
}

export default function PerformancePanel({ stats }) {
  const rows = [
    ['Baseline time', numberValue(stats?.baselineMs, 2, ' ms')],
    ['Optimized time', numberValue(stats?.optimizedMs, 2, ' ms')],
    ['Speedup', numberValue(stats?.speedup, 2, 'x')],
    ['Zero coefficients', numberValue(stats?.zeroCoefficientPercent, 2, '%')],
    ['Huffman bits', stats?.huffmanBitCount ?? '-'],
    ['RLE symbols', stats?.rleSymbolCount ?? '-']
  ];

  return (
    <section className="panel">
      <div className="panel-title">
        <Gauge size={20} />
        <h2>Performance</h2>
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
