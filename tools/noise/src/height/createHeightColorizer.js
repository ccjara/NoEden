export function createHeightColorizer(altitudeOptions) {
  return (value) => {
    if (value <= altitudeOptions.ocean) {
      return [0, 0, 255, 127];
    }
    if (value <= altitudeOptions.shore) {
      return [255, 255, 0, 255];
    }
    if (value <= altitudeOptions.plains) {
      return [0, 255, 0, 255];
    }
    if (value <= altitudeOptions.hills) {
      return [127, 127, 0, 255];
    }
    return [127, 127, 127, 255];
  };
}
