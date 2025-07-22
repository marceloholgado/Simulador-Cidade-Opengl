## Simulador de Cidade em OpenlGl

Jogue em uma simulador implementado em C++ com as bibliotecas do OpenGl.

![alt text](https://github.com/Holgado/Simulador-Cidade-Opengl/blob/main/fotoDoGame.png)

---

### Como compilar

1. Instale o [MinGW](http://www.mingw.org/) e o [FreeGLUT para MinGW](https://www.transmissionzero.co.uk/software/freeglut-devel/).
2. Extraia o FreeGLUT e configure o caminho no arquivo `CMakeLists.txt`.
3. No terminal, execute:

   ```sh
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
   mingw32-make
   ```

O executável será gerado na pasta `build` com o nome `SimuladorCidade.exe`.
