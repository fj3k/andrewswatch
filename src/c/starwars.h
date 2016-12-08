#pragma once

void sw_init(Layer*);
void draw_sw(Layer*, GContext*);

//GColorWhite
static const GPathInfo LEIA_PATH = {
  .num_points = 153,
  .points = (GPoint []) {{56.695, 120.63}, {52.787, 118.664}, {48.901, 115.642}, {43.488, 110.314}, {39.904, 105.957}, {34.358, 98.842}, {28.336, 93.28}, {27.38, 92.626}, {21.148, 89.81}, {20.214, 89.57}, {17.88, 89.419}, {13.529, 89.715}, {12.682, 89.908}, {10.901, 90.422}, {9.682, 90.939}, {8.845, 91.433}, {7.618, 92.135}, {6.792, 91.971}, {6.59, 91.1}, {6.672, 89.952}, {7.416, 86.562}, {8.118, 83.832}, {9.419, 81.15}, {9.78, 80.538}, {10.033, 80.066}, {12.98, 73.835}, {12.903, 73.463}, {12.446, 72.949}, {11.117, 71.147}, {10.872, 70.6}, {10.785, 69.915}, {9.618, 67.997}, {8.505, 66.897}, {7.69, 65.05}, {7.651, 61.878}, {7.481, 60.768}, {6.487, 57.707}, {6.243, 54.464}, {7.163, 51.385}, {7.56, 50.428}, {7.656, 49.516}, {7.451, 48.339}, {6.914, 46.262}, {6.833, 44.949}, {7.393, 42.832}, {7.452, 40.934}, {6.661, 39.236}, {6.241, 37.586}, {6.344, 35.868}, {6.903, 34.772}, {7.185, 34.526}, {7.016, 33.806}, {6.91, 32.719}, {6.24, 31.659}, {5.324, 30.834}, {3.607, 29.504}, {2.527, 28.711}, {0.776, 26.089}, {0.026, 23.514}, {0.175, 22.243}, {0.583, 20.31}, {0, 19.329}, {0.185, 17.485}, {0.423, 16.638}, {0.887, 15.567}, {1.45, 14.563}, {3.797, 12.081}, {5.589, 10.43}, {7.099, 9.08}, {7.657, 8.612}, {8.452, 8.293}, {9.525, 7.95}, {10.552, 7.687}, {12.664, 7.503}, {15.091, 7.697}, {16.174, 7.997}, {17.227, 8.469}, {18.469, 9.41}, {19.365, 9.443}, {23.279, 9.169}, {25.349, 8.233}, {27.403, 7.964}, {29.877, 7.68}, {31.37, 7.433}, {32.926, 7.25}, {34.186, 7.21}, {36.781, 7.704}, {37.578, 7.998}, {39.204, 8.512}, {41.167, 9.025}, {42.912, 9.501}, {44.681, 10.02}, {46.116, 10.546}, {47.361, 11.058}, {48.48, 11.59}, {49.453, 12.094}, {50.413, 12.659}, {52.702, 14.202}, {55.645, 17.413}, {57.059, 20.47}, {57.656, 22.505}, {57.689, 25.557}, {58.075, 27.016}, {59.238, 31.817}, {59.438, 33.73}, {58.688, 37.25}, {57.697, 40.526}, {58.203, 42.785}, {59.904, 46.188}, {60.662, 47.286}, {61.711, 49.33}, {61.278, 50.423}, {59.168, 51.595}, {58.098, 52.091}, {57.37, 52.957}, {57.702, 53.81}, {58.003, 54.467}, {57.669, 55.081}, {56.623, 56.335}, {56.815, 57.011}, {57.007, 57.391}, {56.775, 57.79}, {55.187, 59.308}, {54.599, 59.907}, {54.649, 60.895}, {54.106, 64.252}, {52.511, 65.973}, {49.995, 66.385}, {45.49, 65.329}, {44.356, 64.862}, {42.944, 64.699}, {42.246, 65.599}, {41.22, 67.875}, {40.606, 70.735}, {41.574, 73.053}, {46.042, 77.927}, {47.421, 79.572}, {48.153, 81.761}, {48.466, 83.174}, {49.892, 86.987}, {52.104, 90.271}, {53.589, 92.533}, {55.215, 94.983}, {57.68, 99.837}, {58.44, 101.682}, {59.924, 106.239}, {60.783, 110.408}, {61.06, 112.359}, {60.988, 117.537}, {60.724, 118.585}, {59.469, 120.447}, {59.004, 120.839}, {58.047, 121}}
};
//GColorClear
static const GPathInfo LEIA_HOLE_PATH = {
  .num_points = 17,
  .points = (GPoint []) {{14.921, 59.271}, {14.802, 58.824}, {14.377, 55.906}, {14.8, 52.282}, {15.134, 51.404}, {14.273, 49.219}, {13.2, 47.887}, {12.551, 48.026}, {11.864, 49.023}, {11.923, 50.884}, {12.377, 51.663}, {13.22, 54.028}, {13.109, 56.44}, {12.922, 57.299}, {12.86, 57.911}, {13.395, 58.512}, {14.645, 59.517}}
};

//GColorWhite
static const GPathInfo HAN_PATH = {
  .num_points = 169,
  .points = (GPoint []) {{86.686, 117.093}, {86.367, 116.751}, {85.547, 115.137}, {85.799, 112.406}, {86.555, 109.728}, {88.562, 104.53}, {88.786, 103.982}, {89.302, 102.823}, {90.034, 101.253}, {90.822, 99.605}, {91.491, 98.193}, {92.128, 96.922}, {93.019, 95.18}, {93.521, 94.192}, {95.19, 91.219}, {95.32, 90.976}, {95.583, 90.466}, {96.441, 88.34}, {94.542, 85.157}, {92.868, 82.738}, {94.373, 80.168}, {103.363, 70.75}, {104.197, 68.107}, {103.348, 64.813}, {102.382, 63.414}, {100.684, 63.226}, {97.841, 63.686}, {92.844, 64.142}, {89.066, 63.89}, {86.508, 62.856}, {85.229, 60.347}, {85.437, 57.152}, {85.44, 55.837}, {84.745, 55.183}, {83.894, 54.822}, {83.35, 54.082}, {83.146, 53.659}, {83.443, 53.052}, {84.119, 52.249}, {84.598, 52}, {84.709, 51.383}, {84.085, 51.039}, {83.604, 50.836}, {83.202, 50.564}, {82.228, 49.803}, {81.653, 49.226}, {81.784, 48.437}, {81.971, 46.929}, {81.74, 45.193}, {81.475, 44.878}, {80.094, 44.286}, {79.264, 44.005}, {78.681, 43.738}, {77.394, 42.548}, {77.054, 42.082}, {76.93, 41.611}, {77.024, 41.141}, {77.49, 40.361}, {79.128, 38.204}, {81.388, 35.361}, {82.415, 33.997}, {83.931, 31.233}, {83.539, 30.265}, {83.111, 28.697}, {83.28, 27.003}, {83.575, 25.613}, {84.118, 23.787}, {84.803, 22.603}, {86.279, 19.586}, {85.519, 16.639}, {82.253, 12.506}, {82.289, 10.951}, {83.118, 9.383}, {84.025, 8.174}, {84.912, 6.963}, {86.559, 5.754}, {87.632, 5.39}, {90.429, 5.218}, {91.768, 5.088}, {91.627, 4.136}, {91.65, 3.664}, {92.625, 3.726}, {93.77, 3.778}, {95.185, 3.671}, {96.498, 3.519}, {97.451, 2.414}, {98.202, 2.211}, {98.839, 2.275}, {99.484, 1.704}, {100.332, 1.131}, {102.416, 2.168}, {103.852, 1.671}, {105.68, 0.59}, {106.31, 0.828}, {108.992, 0.85}, {109.658, 0.565}, {110.281, 0.118}, {111.051, 0}, {111.822, 0.117}, {112.444, 0.702}, {113.105, 1.17}, {115.672, 1.179}, {118.589, 1.348}, {121.723, 2.168}, {123.694, 2.96}, {124.994, 3.675}, {127.179, 5.288}, {128.937, 6.265}, {131.324, 7.829}, {132.123, 8.758}, {133.681, 9.886}, {135.292, 11.169}, {136.677, 12.955}, {137.786, 14.892}, {138.45, 16.337}, {139.199, 18.985}, {139.481, 22.692}, {139.665, 26.128}, {140.127, 29.492}, {140.15, 34.09}, {139.623, 38.556}, {139.159, 42.416}, {139.78, 47.256}, {140.156, 48.425}, {139.545, 49.994}, {137.829, 51.694}, {137.536, 52.004}, {138.371, 52.153}, {138.83, 52.323}, {139.491, 55.267}, {139.631, 61.374}, {139.55, 62.162}, {140.925, 63.587}, {142.57, 66.376}, {143.998, 69.512}, {145.451, 73.412}, {146.713, 76.92}, {147.701, 79.855}, {148.728, 83.217}, {149.516, 86.287}, {149.794, 87.558}, {150, 89.398}, {149.837, 91.217}, {149.206, 92.037}, {147.731, 92.124}, {146.48, 91.607}, {145.596, 91.097}, {142.446, 89.253}, {139.672, 88.025}, {137.424, 87.394}, {134.623, 87.244}, {131.823, 87.353}, {130.376, 87.686}, {129.022, 88.054}, {126.278, 89.052}, {123.762, 90.305}, {122.02, 91.279}, {119.873, 92.593}, {119.033, 93.136}, {115.952, 95.392}, {112.78, 97.966}, {109.464, 100.786}, {99.09, 109.683}, {98.016, 110.53}, {97.048, 111.283}, {94.89, 112.908}, {91.73, 115.049}, {87.676, 117.129}, {87.225, 117.212}}
};
//GColorClear
static const GPathInfo HAN_HOLE_PATH = {
  .num_points = 4,
  .points = (GPoint []) {{100.814, 3.316}, {100.994, 2.943}, {100.365, 2.472}, {99.874, 2.965}}
};
