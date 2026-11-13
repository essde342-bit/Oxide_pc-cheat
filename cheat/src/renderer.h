#pragma once

void RendererInit();
void RendererShutdown();
void SetScreenSize(int width, int height);

void DrawRectFilled(float x, float y, float w, float h,
                    float r, float g, float b, float a);

void DrawRectOutline(float x, float y, float w, float h,
                     float r, float g, float b, float a, float thickness);

void DrawLine(float x1, float y1, float x2, float y2,
              float r, float g, float b, float a, float thickness);
