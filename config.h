#pragma once
const unsigned int PIXEL = 32;                 // 每个图块像素
const unsigned int ROW = 500;                   // 环境行数
const unsigned int COL = 500;                   // 环境列数
const double BOUND[4] = { 0, 1, 0, 1 };        // perlin noise 的矩形框范围
const unsigned int TIME_INTERVAL = 500;           // 模拟时间间隔500ms
const unsigned int W_RATIO = 100;					// 优化启发式方法系数
