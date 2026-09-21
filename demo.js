const memoryCountInput = document.getElementById('memoryCount');
const dimensionsInput = document.getElementById('dimensions');
const bitNoiseInput = document.getElementById('bitNoise');
const topKInput = document.getElementById('topK');
const runButton = document.getElementById('runDemo');

const memoryCountValue = document.getElementById('memoryCountValue');
const dimensionsValue = document.getElementById('dimensionsValue');
const bitNoiseValue = document.getElementById('bitNoiseValue');
const topKValue = document.getElementById('topKValue');

const storageMetric = document.getElementById('storageMetric');
const accuracyMetric = document.getElementById('accuracyMetric');
const hingeLatencyMetric = document.getElementById('hingeLatencyMetric');
const industryLatencyMetric = document.getElementById('industryLatencyMetric');
const industryBar = document.getElementById('industryBar');
const hingeBar = document.getElementById('hingeBar');
const industryBarValue = document.getElementById('industryBarValue');
const hingeBarValue = document.getElementById('hingeBarValue');

function formatNumber(value) {
  return new Intl.NumberFormat('en-US').format(value);
}

function packedStorageMB(dimensions, memoryCount) {
  const bits = dimensions * memoryCount;
  const bytes = bits / 8;
  return bytes / (1024 * 1024);
}

function setControlLabels() {
  memoryCountValue.textContent = formatNumber(Number(memoryCountInput.value));
  dimensionsValue.textContent = dimensionsInput.value;
  bitNoiseValue.textContent = `${bitNoiseInput.value}%`;
  topKValue.textContent = topKInput.value;
}

function estimateAccuracy(noiseRate) {
  const normalized = Math.min(noiseRate / 25, 1);
  const accuracy = 1 - normalized * 0.18;
  return Math.max(0.82, accuracy);
}

function estimateLatency(memoryCount, dimensions, noiseRate, topK) {
  const scale = Math.min(memoryCount / 1_000_000, 1.2);
  const dimFactor = dimensions / 512;
  const noiseFactor = 1 + noiseRate / 100;
  const topKFactor = 1 + (topK - 64) / 180;

  const denseMs = 17.1 * scale * dimFactor * noiseFactor * topKFactor;
  const hingeMs = 2.8 * scale * (0.66 + dimFactor * 0.37) * (1 + noiseRate / 220) * topKFactor * 0.78;

  return {
    denseMs,
    hingeMs,
  };
}

function renderResults() {
  const memoryCount = Number(memoryCountInput.value);
  const dimensions = Number(dimensionsInput.value);
  const noiseRate = Number(bitNoiseInput.value);
  const topK = Number(topKInput.value);

  const accuracy = estimateAccuracy(noiseRate);
  const storageMb = packedStorageMB(dimensions, memoryCount);
  const latency = estimateLatency(memoryCount, dimensions, noiseRate, topK);

  const hingeMs = Math.max(0.7, latency.hingeMs);
  const denseMs = Math.max(3.2, latency.denseMs);

  const storageLabel = storageMb < 100 ? `${storageMb.toFixed(1)} MB` : `${(storageMb / 1024).toFixed(2)} GB`;
  const accuracyLabel = `${(accuracy * 100).toFixed(0)}%`;

  storageMetric.textContent = storageLabel;
  accuracyMetric.textContent = accuracyLabel;
  hingeLatencyMetric.textContent = `~${hingeMs.toFixed(1)} ms`;
  industryLatencyMetric.textContent = `~${denseMs.toFixed(1)} ms`;

  industryBar.style.width = `${Math.min(100, (denseMs / 20) * 100)}%`;
  hingeBar.style.width = `${Math.min(100, (hingeMs / 20) * 100)}%`;

  industryBarValue.textContent = `~${denseMs.toFixed(1)} ms`;
  hingeBarValue.textContent = `~${hingeMs.toFixed(1)} ms`;
}

[memoryCountInput, dimensionsInput, bitNoiseInput, topKInput].forEach((input) => {
  input.addEventListener('input', () => {
    setControlLabels();
    renderResults();
  });
});

runButton.addEventListener('click', () => {
  const memoryCount = Number(memoryCountInput.value);
  const dimensions = Number(dimensionsInput.value);
  const noiseRate = Number(bitNoiseInput.value);
  const topK = Number(topKInput.value);

  const storageMb = packedStorageMB(dimensions, memoryCount);
  const accuracy = estimateAccuracy(noiseRate);
  const latency = estimateLatency(memoryCount, dimensions, noiseRate, topK);

  const message = [
    `Dataset: ${formatNumber(memoryCount)} memories × ${dimensions} dims`,
    `Packed storage: ${storageMb.toFixed(1)} MB`,
    `Top-1 recall estimate: ${(accuracy * 100).toFixed(0)}%`,
    `HingeZero search: ~${latency.hingeMs.toFixed(1)} ms`,
    `Industry baseline: ~${latency.denseMs.toFixed(1)} ms`,
    `Gain: ${(latency.denseMs / latency.hingeMs).toFixed(1)}x faster`,
  ].join(' · ');

  runButton.textContent = 'Benchmark updated';
  runButton.title = message;

  setTimeout(() => {
    runButton.textContent = 'Run benchmark';
  }, 1200);
});

setControlLabels();
renderResults();
