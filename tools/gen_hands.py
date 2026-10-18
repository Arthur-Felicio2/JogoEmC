#!/usr/bin/env python3
"""
tools/gen_hands.py — Gerador de sprites de mão em pixel art para Starlight Games.
Gera src/render/HandSprites.h e tools/hands_preview.png sem dependências externas.
"""

import os
import struct
import zlib

WIDTH = 48
HEIGHT = 64

# Índices da paleta:
# 0 = Transparente
# 1 = Contorno da Pele
# 2 = Sombra da Pele
# 3 = Base da Pele
# 4 = Brilho da Pele
# 5 = Contorno da Manga
# 6 = Sombra da Manga
# 7 = Base da Manga
# 8 = Brilho da Manga

def create_grid():
    return [[0 for _ in range(WIDTH)] for _ in range(HEIGHT)]

def fill_rect(grid, x, y, w, h, val):
    for r in range(y, min(HEIGHT, y + h)):
        for c in range(x, min(WIDTH, x + w)):
            if 0 <= r < HEIGHT and 0 <= c < WIDTH:
                grid[r][c] = val

def set_pixel(grid, x, y, val):
    if 0 <= y < HEIGHT and 0 <= x < WIDTH:
        grid[y][x] = val

def draw_sleeve(grid):
    # Manga / Pulso (y = 50 a 63, x = 14 a 34)
    # Contorno
    for y in range(50, 64):
        grid[y][15] = 5
        grid[y][33] = 5
    for x in range(15, 34):
        grid[50][x] = 5
        grid[63][x] = 5
    
    # Preenchimento base da manga
    fill_rect(grid, 16, 51, 17, 12, 7)
    
    # Sombra da manga (lado direito e dobra inferior)
    for y in range(51, 63):
        grid[y][31] = 6
        grid[y][32] = 6
    fill_rect(grid, 18, 59, 14, 2, 6)
    
    # Brilho da manga (lado esquerdo)
    for y in range(51, 62):
        grid[y][16] = 8
        grid[y][17] = 8
        
    # Dobra / Linha de costura
    for x in range(18, 30):
        grid[55][x] = 5

def draw_palm(grid, open_palm=False):
    # Palma (y = 27 a 50)
    # Contorno da palma
    left_x = 13 if open_palm else 15
    right_x = 33
    
    for y in range(28, 51):
        grid[y][left_x] = 1
        grid[y][right_x] = 1
        
    # Preenchimento base
    for y in range(28, 50):
        for x in range(left_x + 1, right_x):
            grid[y][x] = 3
            
    # Sombra lateral direita da palma
    for y in range(29, 50):
        grid[y][right_x - 1] = 2
        grid[y][right_x - 2] = 2
        
    # Brilho lateral esquerdo da palma
    for y in range(30, 48):
        grid[y][left_x + 1] = 4

    # Linhas de expressão da palma
    if open_palm:
        # Linha da vida / vinco
        for i in range(7):
            grid[36 + i][20 + i] = 2
        for i in range(5):
            grid[34 + i][27 - i] = 2

def draw_finger(grid, x, top_y, bot_y, width=5):
    # Dedo estendido de top_y até bot_y
    # Contorno ponta arredondada
    for c in range(x + 1, x + width - 1):
        grid[top_y][c] = 1
    grid[top_y + 1][x] = 1
    grid[top_y + 1][x + width - 1] = 1
    
    # Laterais
    for r in range(top_y + 2, bot_y):
        grid[r][x] = 1
        grid[r][x + width - 1] = 1
        
    # Preenchimento base
    for r in range(top_y + 1, bot_y):
        for c in range(x + 1, x + width - 1):
            grid[r][c] = 3
            
    # Brilho (esquerda do dedo)
    for r in range(top_y + 2, bot_y - 2):
        grid[r][x + 1] = 4
        
    # Sombra (direita do dedo)
    for r in range(top_y + 2, bot_y):
        grid[r][x + width - 2] = 2
        
    # Vinco da articulação
    mid_y = (top_y + bot_y) // 2
    for c in range(x + 1, x + width - 1):
        grid[mid_y][c] = 2

def draw_curled_finger(grid, x, y, width=5, height=8):
    # Dedo enrolado (punho fechado / dedos abaixados)
    # Contorno
    for c in range(x + 1, x + width - 1):
        grid[y][c] = 1
        grid[y + height - 1][c] = 1
    for r in range(y + 1, y + height - 1):
        grid[r][x] = 1
        grid[r][x + width - 1] = 1
        
    # Preenchimento
    for r in range(y + 1, y + height - 1):
        for c in range(x + 1, x + width - 1):
            grid[r][c] = 3
            
    # Brilho superior
    for c in range(x + 1, x + width - 1):
        grid[y + 1][c] = 4
        
    # Sombra inferior
    for c in range(x + 1, x + width - 1):
        grid[y + height - 2][c] = 2

def draw_thumb_folded(grid):
    # Polegar dobrado sobre a palma (x = 13 a 25, y = 34 a 43)
    fill_rect(grid, 14, 35, 11, 7, 3)
    # Contorno
    for x in range(15, 25):
        grid[34][x] = 1
        grid[42][x] = 1
    for y in range(35, 42):
        grid[y][13] = 1
        grid[y][25] = 1
    # Brilho e sombra
    for x in range(16, 23):
        grid[35][x] = 4
    for x in range(16, 24):
        grid[41][x] = 2
    # Unha do polegar
    for y in range(37, 40):
        grid[y][23] = 4
        grid[y][24] = 1

def draw_thumb_spread(grid):
    # Polegar aberto apontando para a esquerda (Pose 5 - Papel)
    # De (14, 40) estendendo para (7, 28)
    coords = [
        (13, 40), (12, 39), (11, 38), (10, 36), (9, 34), (8, 32), (7, 30), (8, 28), (9, 27), (11, 27)
    ]
    # Preenche corpo do polegar
    fill_rect(grid, 9, 30, 6, 9, 3)
    fill_rect(grid, 8, 28, 5, 4, 3)
    
    # Contorno externo
    for x, y in [(7, 30), (6, 31), (7, 33), (8, 35), (9, 37), (10, 39), (11, 41), (12, 42)]:
        set_pixel(grid, x, y, 1)
    for x, y in [(8, 29), (8, 28), (9, 27), (10, 27), (11, 27), (12, 28), (13, 29), (14, 31)]:
        set_pixel(grid, x, y, 1)
    
    # Brilho
    for y in range(29, 36):
        set_pixel(grid, 10, y, 4)
    # Sombra
    for y in range(32, 40):
        set_pixel(grid, 12, y, 2)

# --- Gerador das 6 poses ---

def gen_pose_0():
    # Pose 0: Pedra / Punho Fechado (0 dedos levantados)
    grid = create_grid()
    draw_sleeve(grid)
    draw_palm(grid, open_palm=False)
    
    # 4 dedos enrolados lado a lado no topo da palma
    draw_curled_finger(grid, 15, 23, width=5, height=9) # Indicador
    draw_curled_finger(grid, 19, 21, width=5, height=10) # Médio
    draw_curled_finger(grid, 23, 22, width=5, height=9) # Anelar
    draw_curled_finger(grid, 27, 24, width=5, height=8) # Mindinho
    
    # Polegar dobrado sobre os dedos
    draw_thumb_folded(grid)
    return grid

def gen_pose_1():
    # Pose 1: 1 dedo levantado (Indicador estendido)
    grid = create_grid()
    draw_sleeve(grid)
    draw_palm(grid, open_palm=False)
    
    # Indicador estendido
    draw_finger(grid, 16, top_y=8, bot_y=28, width=5)
    
    # Outros 3 dedos enrolados
    draw_curled_finger(grid, 20, 23, width=5, height=9)
    draw_curled_finger(grid, 24, 24, width=5, height=8)
    draw_curled_finger(grid, 28, 25, width=5, height=7)
    
    draw_thumb_folded(grid)
    return grid

def gen_pose_2():
    # Pose 2: Tesoura / 2 dedos levantados em V
    grid = create_grid()
    draw_sleeve(grid)
    draw_palm(grid, open_palm=False)
    
    # Dedo Indicador inclinado à esquerda
    for y in range(8, 28):
        x = 13 + (y - 8) // 4
        fill_rect(grid, x, y, 4, 1, 3)
        grid[y][x - 1] = 1
        grid[y][x + 4] = 1
        grid[y][x] = 4
        grid[y][x + 3] = 2
    # Ponta do indicador
    grid[7][14] = 1
    grid[7][15] = 1
    grid[8][13] = 1
    
    # Dedo Médio inclinado à direita
    for y in range(6, 28):
        x = 24 - (y - 6) // 5
        fill_rect(grid, x, y, 4, 1, 3)
        grid[y][x - 1] = 1
        grid[y][x + 4] = 1
        grid[y][x] = 4
        grid[y][x + 3] = 2
    # Ponta do médio
    grid[5][23] = 1
    grid[5][24] = 1
    grid[6][25] = 1
    
    # Anelar e mindinho enrolados
    draw_curled_finger(grid, 25, 24, width=5, height=9)
    draw_curled_finger(grid, 29, 25, width=5, height=8)
    
    draw_thumb_folded(grid)
    return grid

def gen_pose_3():
    # Pose 3: 3 dedos levantados (Indicador, Médio, Anelar)
    grid = create_grid()
    draw_sleeve(grid)
    draw_palm(grid, open_palm=False)
    
    # 3 dedos estendidos
    draw_finger(grid, 16, top_y=8, bot_y=28, width=5)
    draw_finger(grid, 21, top_y=6, bot_y=28, width=5)
    draw_finger(grid, 26, top_y=9, bot_y=28, width=5)
    
    # Mindinho enrolado
    draw_curled_finger(grid, 30, 25, width=4, height=8)
    
    draw_thumb_folded(grid)
    return grid

def gen_pose_4():
    # Pose 4: 4 dedos levantados (Indicador, Médio, Anelar, Mindinho)
    grid = create_grid()
    draw_sleeve(grid)
    draw_palm(grid, open_palm=False)
    
    draw_finger(grid, 15, top_y=9, bot_y=28, width=5)
    draw_finger(grid, 20, top_y=6, bot_y=28, width=5)
    draw_finger(grid, 25, top_y=8, bot_y=28, width=5)
    draw_finger(grid, 30, top_y=13, bot_y=28, width=4)
    
    draw_thumb_folded(grid)
    return grid

def gen_pose_5():
    # Pose 5: Papel / Mão aberta (5 dedos estendidos)
    grid = create_grid()
    draw_sleeve(grid)
    draw_palm(grid, open_palm=True)
    
    # Polegar aberto à esquerda
    draw_thumb_spread(grid)
    
    # 4 dedos abertos e espalhados
    draw_finger(grid, 15, top_y=9, bot_y=28, width=5)
    draw_finger(grid, 20, top_y=6, bot_y=28, width=5)
    draw_finger(grid, 25, top_y=8, bot_y=28, width=5)
    draw_finger(grid, 30, top_y=13, bot_y=28, width=4)
    
    return grid

POSES = [
    gen_pose_0(),
    gen_pose_1(),
    gen_pose_2(),
    gen_pose_3(),
    gen_pose_4(),
    gen_pose_5(),
]

# --- Paletas de Cores (RGBA) ---
PALETTES = {
    "skins": [
        # 0: Claro
        {"name": "Claro", "outline": (130, 75, 55, 255), "shadow": (220, 165, 135, 255), "base": (252, 215, 190, 255), "highlight": (255, 238, 225, 255)},
        # 1: Médio Claro
        {"name": "Médio Claro", "outline": (115, 70, 40, 255), "shadow": (195, 145, 90, 255), "base": (238, 192, 132, 255), "highlight": (255, 222, 172, 255)},
        # 2: Moreno
        {"name": "Moreno", "outline": (90, 50, 25, 255), "shadow": (160, 105, 55, 255), "base": (205, 145, 90, 255), "highlight": (232, 175, 125, 255)},
        # 3: Escuro
        {"name": "Escuro", "outline": (45, 28, 18, 255), "shadow": (88, 52, 32, 255), "base": (130, 78, 48, 255), "highlight": (160, 105, 70, 255)},
    ],
    "sleeves": [
        # 0: Azul (Jogador 1)
        {"name": "Azul", "outline": (18, 38, 85, 255), "shadow": (32, 72, 155, 255), "base": (52, 112, 215, 255), "highlight": (95, 160, 250, 255)},
        # 1: Vermelha (Jogador 2)
        {"name": "Vermelha", "outline": (75, 18, 22, 255), "shadow": (145, 32, 42, 255), "base": (210, 48, 62, 255), "highlight": (250, 105, 115, 255)},
    ]
}

def export_header(output_path):
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write("#pragma once\n")
        f.write("// HandSprites.h — Gerado automaticamente por tools/gen_hands.py\n")
        f.write("// Starlight Company — Mãos em pixel art (48x64 px, 6 poses, 4 tons de pele, 2 mangas)\n\n")
        f.write("#include <cstdint>\n\n")
        f.write("namespace sl {\n\n")
        f.write("constexpr int HAND_WIDTH = 48;\n")
        f.write("constexpr int HAND_HEIGHT = 64;\n")
        f.write("constexpr int PIVOT_X = 24;\n")
        f.write("constexpr int PIVOT_Y = 60;\n")
        f.write("constexpr int POSE_COUNT = 6;\n")
        f.write("constexpr int SKIN_COUNT = 4;\n")
        f.write("constexpr int SLEEVE_COUNT = 2;\n\n")
        
        f.write("struct ColorRGBA {\n")
        f.write("    uint8_t r, g, b, a;\n")
        f.write("};\n\n")
        
        f.write("struct SkinPalette {\n")
        f.write("    const char* name;\n")
        f.write("    ColorRGBA outline;\n")
        f.write("    ColorRGBA shadow;\n")
        f.write("    ColorRGBA base;\n")
        f.write("    ColorRGBA highlight;\n")
        f.write("};\n\n")
        
        f.write("struct SleevePalette {\n")
        f.write("    const char* name;\n")
        f.write("    ColorRGBA outline;\n")
        f.write("    ColorRGBA shadow;\n")
        f.write("    ColorRGBA base;\n")
        f.write("    ColorRGBA highlight;\n")
        f.write("};\n\n")
        
        f.write("inline const SkinPalette kSkins[SKIN_COUNT] = {\n")
        for s in PALETTES["skins"]:
            f.write(f'    {{ "{s["name"]}", {{{s["outline"][0]}, {s["outline"][1]}, {s["outline"][2]}, {s["outline"][3]}}}, ')
            f.write(f'{{{s["shadow"][0]}, {s["shadow"][1]}, {s["shadow"][2]}, {s["shadow"][3]}}}, ')
            f.write(f'{{{s["base"][0]}, {s["base"][1]}, {s["base"][2]}, {s["base"][3]}}}, ')
            f.write(f'{{{s["highlight"][0]}, {s["highlight"][1]}, {s["highlight"][2]}, {s["highlight"][3]}}} }},\n')
        f.write("};\n\n")
        
        f.write("inline const SleevePalette kSleeves[SLEEVE_COUNT] = {\n")
        for sl in PALETTES["sleeves"]:
            f.write(f'    {{ "{sl["name"]}", {{{sl["outline"][0]}, {sl["outline"][1]}, {sl["outline"][2]}, {sl["outline"][3]}}}, ')
            f.write(f'{{{sl["shadow"][0]}, {sl["shadow"][1]}, {sl["shadow"][2]}, {sl["shadow"][3]}}}, ')
            f.write(f'{{{sl["base"][0]}, {sl["base"][1]}, {sl["base"][2]}, {sl["base"][3]}}}, ')
            f.write(f'{{{sl["highlight"][0]}, {sl["highlight"][1]}, {sl["highlight"][2]}, {sl["highlight"][3]}}} }},\n')
        f.write("};\n\n")
        
        f.write("// Dados indexados dos sprites das 6 poses (48x64 cada)\n")
        f.write("inline const uint8_t kPosePixels[POSE_COUNT][HAND_WIDTH * HAND_HEIGHT] = {\n")
        for p_idx, grid in enumerate(POSES):
            f.write(f"    // Pose {p_idx}\n    {{\n")
            flat = [val for row in grid for val in row]
            for i in range(0, len(flat), 24):
                chunk = ", ".join(str(v) for v in flat[i:i+24])
                f.write(f"        {chunk},\n")
            f.write("    },\n")
        f.write("};\n\n")
        f.write("} // namespace sl\n")

# --- Exportador PNG sem dependências externas ---
def write_png(filepath, width, height, rgba_data):
    def make_chunk(chunk_type, data):
        c = chunk_type + data
        crc = zlib.crc32(c) & 0xffffffff
        return struct.pack(">I", len(data)) + c + struct.pack(">I", crc)
    
    header = b"\x89PNG\r\n\x1a\n"
    ihdr = make_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
    
    raw_scanlines = bytearray()
    for y in range(height):
        raw_scanlines.append(0) # Filtro None
        offset = y * width * 4
        raw_scanlines.extend(rgba_data[offset : offset + width * 4])
        
    idat = make_chunk(b"IDAT", zlib.compress(bytes(raw_scanlines), level=9))
    iend = make_chunk(b"IEND", b"")
    
    with open(filepath, "wb") as f:
        f.write(header + ihdr + idat + iend)

def export_preview(output_png):
    # Cria uma imagem de preview mostrando as 6 poses com as 4 peles e 2 mangas
    # 6 colunas de poses, 4 linhas de peles (esquerda com manga azul, direita com manga vermelha)
    scale = 3
    spacing = 8
    cols = 6
    rows = 4
    
    cell_w = WIDTH * scale
    cell_h = HEIGHT * scale
    
    total_w = cols * (cell_w + spacing) + spacing
    total_h = rows * (cell_h + spacing) + spacing
    
    # Fundo xadrez cinza escuro / mesa verde suave
    bg_color = (18, 55, 42, 255)
    img_data = bytearray(total_w * total_h * 4)
    for i in range(0, len(img_data), 4):
        img_data[i] = bg_color[0]
        img_data[i+1] = bg_color[1]
        img_data[i+2] = bg_color[2]
        img_data[i+3] = bg_color[3]
        
    for skin_idx in range(rows):
        sleeve_idx = 0 if skin_idx < 2 else 1
        skin = PALETTES["skins"][skin_idx]
        sleeve = PALETTES["sleeves"][sleeve_idx]
        
        color_map = {
            0: (0, 0, 0, 0),
            1: skin["outline"],
            2: skin["shadow"],
            3: skin["base"],
            4: skin["highlight"],
            5: sleeve["outline"],
            6: sleeve["shadow"],
            7: sleeve["base"],
            8: sleeve["highlight"],
        }
        
        for pose_idx in range(cols):
            grid = POSES[pose_idx]
            start_x = spacing + pose_idx * (cell_w + spacing)
            start_y = spacing + skin_idx * (cell_h + spacing)
            
            for gy in range(HEIGHT):
                for gx in range(WIDTH):
                    c = color_map.get(grid[gy][gx], (0, 0, 0, 0))
                    if c[3] == 0:
                        continue
                    for sy in range(scale):
                        for sx in range(scale):
                            px = start_x + gx * scale + sx
                            py = start_y + gy * scale + sy
                            idx = (py * total_w + px) * 4
                            img_data[idx] = c[0]
                            img_data[idx+1] = c[1]
                            img_data[idx+2] = c[2]
                            img_data[idx+3] = c[3]
                            
    os.makedirs(os.path.dirname(output_png), exist_ok=True)
    write_png(output_png, total_w, total_h, bytes(img_data))
    print(f"Preview salvo com sucesso em {output_png} ({total_w}x{total_h} px)")

if __name__ == "__main__":
    header_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "src", "render", "HandSprites.h"))
    preview_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "tools", "hands_preview.png"))
    
    print(f"Gerando {header_path}...")
    export_header(header_path)
    print("HandSprites.h gerado com sucesso!")
    
    print(f"Gerando preview {preview_path}...")
    export_preview(preview_path)
    print("Concluído!")
