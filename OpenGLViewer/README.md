# OpenGL 3D Scene Viewer – Grau A
**UNISINOS – Escola Politécnica** | Computação Gráfica

---

## ⚙️ Configuração no Windows (VS Code + MinGW + vcpkg)

O erro `nmake not found` acontece porque o CMake tentou usar o compilador da Microsoft. Siga os passos abaixo para configurar corretamente com **MinGW** e **vcpkg**.

---

### Passo 1 – Instalar o MSYS2 / MinGW-w64

1. Baixe e instale o **MSYS2**: https://www.msys2.org/
2. Abra o terminal **MSYS2 MinGW 64-bit** e execute:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make
   ```
3. Adicione ao **PATH do Windows** (Painel de Controle → Variáveis de Ambiente):
   ```
   C:\msys64\mingw64\bin
   ```

### Passo 2 – Instalar o vcpkg

Abra o **PowerShell** como administrador:
```powershell
cd C:\
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Adicione a variável de ambiente VCPKG_ROOT = C:\vcpkg
[System.Environment]::SetEnvironmentVariable("VCPKG_ROOT","C:\vcpkg","Machine")
```

> **Reinicie o VS Code** depois de definir a variável.

### Passo 3 – Instalar as dependências via vcpkg

```powershell
cd C:\vcpkg
.\vcpkg install glfw3:x64-mingw-dynamic
.\vcpkg install glm:x64-mingw-dynamic
.\vcpkg install assimp:x64-mingw-dynamic
```

> Se preferir o triplet estático, use `x64-mingw-static` — mas precisará ajustar o CMakeLists.

### Passo 4 – Extensões do VS Code

Instale as extensões:
- **C/C++** (Microsoft)
- **CMake Tools** (Microsoft)
- **CMake** (twxs) — syntax highlight

### Passo 5 – Abrir e compilar o projeto

1. Abra a pasta `OpenGLViewer` no VS Code (`File → Open Folder`)
2. Pressione `Ctrl+Shift+P` → `CMake: Configure`
   - Se pedir o kit (compilador), selecione **GCC for x86_64-w64-mingw32**
3. Pressione `Ctrl+Shift+P` → `CMake: Build` (ou `Ctrl+F7`)
4. O executável será gerado em `build/OpenGLViewer.exe`

### Passo 6 – Rodar

```powershell
cd build
.\OpenGLViewer.exe
# Ou com seus modelos:
.\OpenGLViewer.exe ..\assets\models\meu_modelo.obj
```

---

## 🐧 Linux (Ubuntu/Debian)

```bash
sudo apt install cmake build-essential libglfw3-dev libglm-dev libassimp-dev libgl1-mesa-dev

cmake -B build
cmake --build build --parallel
./build/OpenGLViewer
```

---

## 🍎 macOS

```bash
brew install cmake glfw glm assimp
cmake -B build
cmake --build build --parallel
./build/OpenGLViewer
```

---

## 🎮 Controles

### Câmera (FPS)
| Tecla | Ação |
|---|---|
| `W / A / S / D` | Mover câmera |
| `Espaço / Ctrl` | Subir / Descer |
| Mouse | Olhar ao redor |
| Scroll | Zoom |
| `ESC` | Liberar/capturar mouse |
| `P` | Alternar Perspectiva ↔ Ortográfica |

### Seleção de Objetos
| Tecla | Ação |
|---|---|
| `TAB` | Próximo objeto |
| `Q` | Objeto anterior |

*(Objeto selecionado fica com borda amarela)*

### Transformações (objeto selecionado)
| Tecla | Ação |
|---|---|
| `I / K` | Rotação eixo X |
| `J / L` | Rotação eixo Y |
| `U / O` | Rotação eixo Z |
| `← →` | Translação X |
| `↑ ↓` | Translação Z |
| `R / Y` | Translação Y (cima/baixo) |
| `+ / -` | Escala uniforme |

### Display
| Tecla | Ação |
|---|---|
| `F` | Wireframe sobreposto |

---

## 🏗️ Arquitetura

```
CPU (C++)                          GPU (GLSL)
─────────────────────              ──────────────────────
Assimp lê .obj/.ply
  │
  ▼
Model::processMesh()
  │  std::vector<Vertex>
  ▼  {pos, normal, uv}
Mesh::setup()
  │  glGenVertexArrays
  │  glGenBuffers (VBO/EBO)
  │  glVertexAttribPointer
  ▼
Mesh::draw()                ──►   phong.vert
  glDrawElements                    │ gl_Position = proj * view * model * pos
                                    ▼
Camera::getView()           ──►   phong.frag
Camera::getProjection()             │ Phong = Ambiente + Difusa + Especular
                                    │ com atenuação da luz pontual
                                    ▼
Scene::draw()               ──►   Framebuffer (tela)
```

### Modelo de Iluminação de Phong
Implementado em `shaders/phong.frag`:

- **Ambiente:** `ka × La × color`
- **Difusa:** `kd × Ld × max(N·L, 0) × color`
- **Especular:** `ks × Ls × max(V·R, 0)^shininess`
- **Atenuação:** `1 / (c + l·d + q·d²)`

---

## ✅ Requisitos Implementados

| Requisito | Status |
|---|---|
| Parser .obj e .ply via Assimp | ✅ |
| Vértices, faces (triângulos), normais, texcoords | ✅ |
| VAO + VBO + EBO | ✅ |
| Múltiplos objetos simultâneos | ✅ |
| Seleção via teclado | ✅ |
| Rotação X, Y, Z | ✅ |
| Translação X, Y, Z | ✅ |
| Escala uniforme | ✅ |
| Câmera FPS (teclado + mouse) | ✅ |
| Perspectiva e Ortográfica | ✅ |
| Phong (ambiente + difusa + especular) | ✅ |
| Fonte de luz pontual configurável | ✅ |
| ka, kd, ks configuráveis por objeto | ✅ |
| Modo sólido com iluminação | ✅ |
| Wireframe sobreposto (tecla F) | ✅ |
| Grid de chão (extra) | ✅ |

---

## Componentes

- Nome Completo do Aluno 1
- Nome Completo do Aluno 2 *(se dupla)*
