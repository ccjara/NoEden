export const BiomeType = {
  None: 0,
  Rainforest: 1,
  Savanna: 2,
  TemperateForest: 3,
  Grassland: 4,
  Desert: 5,
  Steppe: 6,
  Taiga: 7,
  Tundra: 8,
  Swamp: 9,
  Marsh: 10,
  Coastal: 11,
  Ocean: 12,
  Alpine: 13,
};

export const biomeCategories = {
  [BiomeType.None]: {
    label: 'None',
    color: [255, 0, 255, 255],
  },
  [BiomeType.Rainforest]: {
    label: 'Rainforest',
    color: [0, 63, 0, 255],
  },
  [BiomeType.Savanna]: {
    label: 'Savanna',
    color: [63, 127, 0, 255],
  },
  [BiomeType.TemperateForest]: {
    label: 'Temperate Forest',
    color: [0, 127, 0, 255],
  },
  [BiomeType.Grassland]: {
    label: 'Grassland',
    color: [0, 255, 0, 255],
  },
  [BiomeType.Desert]: {
    label: 'Desert',
    color: [255, 255, 0, 255],
  },
  [BiomeType.Steppe]: {
    label: 'Steppe',
    color: [127, 127, 63, 255],
  },
  [BiomeType.Taiga]: {
    label: 'Taiga',
    color: [63, 63, 0, 255],
  },
  [BiomeType.Tundra]: {
    label: 'Tundra',
    color: [0, 127, 63, 255],
  },
  [BiomeType.Swamp]: {
    label: 'Swamp',
    color: [63, 127, 0, 255],
  },
  [BiomeType.Marsh]: {
    label: 'Marsh',
    color: [63, 127, 63, 255],
  },
  [BiomeType.Coastal]: {
    label: 'Coastal',
    color: [0, 127, 255, 255],
  },
  [BiomeType.Ocean]: {
    label: 'Ocean',
    color: [0, 0, 255, 255],
  },
  [BiomeType.Alpine]: {
    label: 'Alpine',
    color: [127, 127, 127, 255],
  },
};
