import { Cpu, Database, Repeat } from 'lucide-react';

export default function MemoryLayoutExplanation() {
  const cards = [
    {
      icon: <Database size={20} />,
      title: 'Baseline layout',
      text: 'The baseline path favors clarity: each 8x8 block is gathered directly from image planes and processed with direct DCT math. It is easy to follow, but repeated scattered reads are less friendly to CPU caches.'
    },
    {
      icon: <Cpu size={20} />,
      title: 'Optimized layout',
      text: 'The optimized path keeps Y, Cb, and Cr in flat contiguous buffers. Scanline block traversal makes nearby pixels sit close together in memory, which helps the CPU reuse cache lines.'
    },
    {
      icon: <Repeat size={20} />,
      title: 'Reusable buffers',
      text: 'The inner loop reuses fixed local arrays for the source block, DCT block, int16_t quantized coefficients, and reconstruction. That avoids heap allocation during block processing.'
    }
  ];

  return (
    <section className="memory-section">
      <div className="section-heading">
        <p className="eyebrow">Memory optimization</p>
        <h2>Why cache-aware storage matters</h2>
      </div>
      <div className="memory-grid">
        {cards.map((card) => (
          <article className="memory-card" key={card.title}>
            <div className="icon-box">{card.icon}</div>
            <h3>{card.title}</h3>
            <p>{card.text}</p>
          </article>
        ))}
      </div>
    </section>
  );
}
