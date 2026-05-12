# Exercício Grau A – Computação Gráfica
## Selecionando e Aplicando Transformações em Objetos 3D

---

## Descrição

Programa em **C++ com OpenGL moderna (3.3 core profile)** que carrega modelos 3D (`.obj`), exibe múltiplos objetos organizados em grade, permite seleção cíclica e aplicação de transformações (rotação, translação e escala) via teclado.

---

## Dependências

| Biblioteca | Versão mínima |
|---|---|
| OpenGL | 3.3 |
| GLFW | 3.x |
| GLEW | 2.x |
| GLM | 0.9.x |
| CMake | 3.10 |

### Instalação no Ubuntu/Debian
```bash
sudo apt update
sudo apt install cmake build-essential libglfw3-dev libglew-dev libglm-dev
```

### Instalação no Fedora/RHEL
```bash
sudo dnf install cmake gcc-c++ glfw-devel glew-devel glm-devel
```

### Instalação no macOS (Homebrew)
```bash
brew install cmake glfw glew glm
```

### Windows (vcpkg)
```powershell
vcpkg install glfw3 glew glm
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake ..
```

---

## Compilação

```bash
# Na raiz do projeto:
mkdir build && cd build
cmake ..
make -j$(nproc)
```

O executável `cg_exercise` será gerado em `build/`.

---

## Uso

### Sem arquivos .obj (usa formas procedurais: cubo, esfera, pirâmide)
```bash
./cg_exercise
```

### Com arquivos .obj
```bash
./cg_exercise suzanne.obj bunny.obj teapot.obj skull.obj
```

Recomenda-se usar os modelos do repositório da disciplina. Se menos de 4 arquivos forem fornecidos, os slots restantes são preenchidos com formas procedurais.

---

## Controles

| Tecla | Ação |
|---|---|
| **TAB** ou **N** | Selecionar próximo objeto (cíclico) |
| **HOME** | Resetar transformações do objeto selecionado |
| **F** | Toggle overlay wireframe em todos os objetos |
| **ESC** | Sair |

### Rotação (segure **R** + eixo)
| Tecla | Ação |
|---|---|
| R + X | Rotacionar +X |
| R + Z | Rotacionar -X |
| R + Y | Rotacionar +Y |
| R + U | Rotacionar -Y |
| R + C | Rotacionar +Z |
| R + V | Rotacionar -Z |

### Translação (segure **T** + direção)
| Tecla | Ação |
|---|---|
| T + ←→ / A D | Transladar X |
| T + ↑↓ / W S | Transladar Y |
| T + Q / E | Transladar Z (frente/trás) |

### Escala (segure **S** + modificador)
| Tecla | Ação |
|---|---|
| S + = / KP+ | Escala uniforme + |
| S + - / KP- | Escala uniforme - |
| S + X / Z | Escala +X / -X |
| S + Y / U | Escala +Y / -Y |
| S + C / V | Escala +Z / -Z |

---

## Funcionalidades implementadas

1. ✅ Leitura de arquivos `.obj` (suporta `v`, `vn`, `f` com faces triangulares e poligonais)
2. ✅ Exibição de múltiplos objetos organizados em grade 2×2
3. ✅ Seleção cíclica via teclado (TAB/N) com realce visual do selecionado
4. ✅ Rotação nos eixos X, Y e Z
5. ✅ Translação nos eixos X, Y e Z (setas + WASD)
6. ✅ Escala nos eixos X, Y, Z e uniforme
7. ✅ **DESAFIO**: Wireframe por cima com `glPolygonOffset` (evita z-fighting)

## Detalhes técnicos

- Shader com iluminação difusa simples (Phong simplificado sem especular)
- Normalização automática do modelo `.obj` para caber em [-1,1]
- Triangulação automática de polígonos convexos (fan triangulation)
- Informações de transform exibidas no título da janela em tempo real
- `struct SceneObject` encapsula geometria (VAO/VBO), posição, rotação, escala e cor

---

## Autores

- **[Seu Nome Completo]**
- **[Nome do Parceiro (se dupla)]**

---

## Estrutura do projeto

```
cg_exercise/
├── main.cpp          # Código-fonte principal
├── CMakeLists.txt    # Build system
└── README.md         # Este arquivo
```
