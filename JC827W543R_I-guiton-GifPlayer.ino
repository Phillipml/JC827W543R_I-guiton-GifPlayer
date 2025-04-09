#include <SPIFFS.h>
#include <AnimatedGIF.h>
#include "device_config.h"

#define DISPLAY_WIDTH  480
#define DISPLAY_HEIGHT 270

AnimatedGIF gif;
File f;

// ----- Funções de acesso para SPIFFS -----
void *GIFOpenFile(const char *fname, int32_t *pSize) {
  f = SPIFFS.open(fname);
  if (f) {
    *pSize = f.size();
    return (void *)&f;
  }
  return NULL;
}

void GIFCloseFile(void *pHandle) {
  File *fp = static_cast<File *>(pHandle);
  if (fp) fp->close();
}

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
  File *fp = static_cast<File *>(pFile->fHandle);
  int32_t iBytesRead = iLen;
  if ((pFile->iSize - pFile->iPos) < iLen)
    iBytesRead = pFile->iSize - pFile->iPos;
  if (iBytesRead <= 0) return 0;
  iBytesRead = (int32_t)fp->read(pBuf, iBytesRead);
  pFile->iPos = fp->position();
  return iBytesRead;
}

int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
  File *fp = static_cast<File *>(pFile->fHandle);
  fp->seek(iPosition);
  pFile->iPos = fp->position();
  return pFile->iPos;
}

// Função auxiliar para trocar bytes (RGB565 endian swap)
uint16_t swap565(uint16_t c) {
  return (c << 8) | (c >> 8);
}

// ----- Função de desenho do frame do GIF -----
void GIFDraw(GIFDRAW *pDraw) {
  uint8_t *s = pDraw->pPixels;
  uint16_t *pPal = pDraw->pPalette;
  static uint16_t lineBuf[DISPLAY_WIDTH];
  int x = pDraw->iX;
  int y = pDraw->iY + pDraw->y;
  int width = pDraw->iWidth;

  if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
  if (width + x > DISPLAY_WIDTH) width = DISPLAY_WIDTH - x;

  for (int i = 0; i < width; i++) {
    uint8_t idx = s[i];
    if (pDraw->ucHasTransparency && idx == pDraw->ucTransparent) {
      lineBuf[i] = 0x0000;
    } else {
      lineBuf[i] = swap565(pPal[idx]);  // <- Corrigido aqui!
    }
  }

  gfx->draw16bitRGBBitmap(x, y, lineBuf, width, 1);
}

// ----- Setup -----
void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao iniciar SPIFFS!");
    return;
  }
  Serial.println("SPIFFS pronto.");

  gfx->begin();
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);

  gfx->fillScreen(BLACK);
  gif.begin(BIG_ENDIAN_PIXELS);  // <- Usa big-endian da lib, a gente inverte no código
}

// ----- Loop -----
void loop() {
  if (gif.open("/output.gif", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    Serial.println("GIF carregado com sucesso!");
    while (gif.playFrame(true, NULL)) {
      // Delay automático entre frames
    }
    gif.close();
  } else {
    int err = gif.getLastError();
    Serial.printf("Erro ao abrir o GIF! Código: %d\n", err);
    gfx->fillScreen(BLACK);
    gfx->setCursor(0, 0);
    gfx->setTextColor(RED);
    gfx->setTextSize(2);
    gfx->println("Erro ao abrir GIF");
    gfx->printf("Codigo: %d\n", err);
    delay(5000);
  }

  delay(2000);
}
