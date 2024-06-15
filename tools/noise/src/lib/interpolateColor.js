export function interpolateColor(value, colorStops) {
  let startColor = colorStops[0].color;
  let endColor = colorStops[colorStops.length - 1].color;
  let startStop = colorStops[0].stop;
  let endStop = colorStops[colorStops.length - 1].stop;

  for (let i = 1; i < colorStops.length; i++) {
    if (value < colorStops[i].stop) {
      startColor = colorStops[i - 1].color;
      endColor = colorStops[i].color;
      startStop = colorStops[i - 1].stop;
      endStop = colorStops[i].stop;
      break;
    }
  }

  const ratio = (value - startStop) / (endStop - startStop);

  return [
    Math.round(startColor[0] + ratio * (endColor[0] - startColor[0])),
    Math.round(startColor[1] + ratio * (endColor[1] - startColor[1])),
    Math.round(startColor[2] + ratio * (endColor[2] - startColor[2])),
    Math.round(startColor[3] + ratio * (endColor[3] - startColor[3])),
  ];
}
