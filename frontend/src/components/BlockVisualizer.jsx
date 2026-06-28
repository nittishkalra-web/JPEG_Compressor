import { Grid3X3 } from 'lucide-react';

function Matrix({ title, values, decimals = 0 }) {
  return (
    <div className="matrix-wrap">
      <h3>{title}</h3>
      <div className="matrix">
        {(values || Array(64).fill(null)).map((value, index) => (
          <span key={index}>{value === null ? '-' : Number(value).toFixed(decimals)}</span>
        ))}
      </div>
    </div>
  );
}

export default function BlockVisualizer({ block }) {
  return (
    <section className="panel block-panel">
      <div className="panel-title">
        <Grid3X3 size={20} />
        <h2>8x8 Block Visualization</h2>
      </div>
      <div className="matrix-grid">
        <Matrix title="Original Y" values={block?.originalY} decimals={0} />
        <Matrix title="DCT coefficients" values={block?.dct} decimals={1} />
        <Matrix title="Quantized" values={block?.quantized} decimals={0} />
      </div>
      <div className="sequence-grid">
        <div>
          <h3>Zigzag sequence</h3>
          <div className="token-box">{block?.zigzag?.join(', ') || '-'}</div>
        </div>
        <div>
          <h3>RLE output</h3>
          <div className="token-box">{block?.rle?.join('  ') || '-'}</div>
        </div>
      </div>
    </section>
  );
}
