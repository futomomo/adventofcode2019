typedef struct Program {
  unsigned int instPtr;
  int prgSize;
  int* prg;
} program;


program* prgLoad(const char* filename, const char* mode);
void prgRun(program* p);
void prgFree(program* p);
