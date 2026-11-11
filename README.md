# 🌅 SUNSET BLOCKS

**Controlador físico + block puzzle arcade estilo 90s.**
Un slider, una perilla y un botón arcade, montados en una caja impresa en 3D, controlan un puzzle de bloques pixelado que corre en el navegador. A medida que subes de nivel, el sol del juego se va poniendo… y el sol de la caja también.

> Proyecto para **Diseño de Controlador Físico · Unidad 2 · Universidad Adolfo Ibáñez**
> Autora: **[antonia aracena]** · 2026


## 🎬 Video

[![Ver el video del proyecto](docs/juego_partida.png)](https://youtu.be/TU_VIDEO_AQUI)

> 👉 Reemplaza `https://www.youtube.com/shorts/JC0CRTiaetk´
---

## 📑 Contenido

1. [Concepto](#-concepto)
2. [Cómo se juega](#-cómo-se-juega)
3. [Hardware](#-hardware)
4. [Diagrama esquemático del shield](#-diagrama-esquemático-del-shield)
5. [Carcasa](#-carcasa)
6. [Software](#-software)
7. [Cómo usarlo](#-cómo-usarlo)
8. [Proceso de fabricación](#-proceso-de-fabricación)
9. [Estructura del repositorio](#-estructura-del-repositorio)
10. [Cumplimiento de la pauta](#-cumplimiento-de-la-pauta)

---

## 💡 Concepto

Sunset Blocks traduce los tres gestos básicos de un block puzzle a tres controles físicos:

| Gesto | Control | Tipo de señal |
|---|---|---|
| **Mover** la pieza | Slider (potenciómetro deslizante) | Continua · `A0` |
| **Girar** la pieza | Perilla (potenciómetro giratorio) | Continua · `A1` |
| **Dejar caer** la pieza | Botón arcade | Discreta · `D2` |

La estética toma el **atardecer** como hilo conductor: el cielo del juego pasa de la tarde a la noche con cada nivel, y un **sol pixelado calado en la tapa**, iluminado por un LED, reacciona a lo que pasa en pantalla.

---

## 🕹️ Cómo se juega

- **Slider:** la pieza sigue la posición del slider, columna por columna.
- **Perilla:** cada pequeño giro rota la pieza hacia ese lado.
- **Botón:** la pieza cae de golpe. También sirve para empezar y para reiniciar.
- Completa líneas para sumar puntos. Cada 10 líneas subes de nivel, el juego se acelera y el sol se pone.

**El sol de la caja (LED):**

| En el juego | El sol |
|---|---|
| Pantalla de inicio | respira suave |
| Jugando | encendido fijo |
| Haces una línea | destella |
| Haces 4 líneas | parpadea fuerte |
| Game over | se apaga lentamente (se pone el sol) |

---

## 🔌 Hardware

| Cant. | Componente | Detalle |
|---|---|---|
| 1 | Arduino UNO | + cable USB |
| 1 | Shield en PCB perforada | con headers macho para montarlo sobre el Arduino |
| 1 | Potenciómetro deslizante 10 kΩ | ranura de 70 × 10 mm en la tapa |
| 1 | Potenciómetro giratorio 10 kΩ | eje de 6 mm, rosca M7 |
| 1 | Botón arcade | Ø 25 mm |
| 1 | LED 5 mm | sol, detrás de un difusor impreso |
| 1 | LED 5 mm (opcional) | luz del botón |
| 2 | Resistencias 220 Ω | para los LEDs, soldadas en el shield |
| 5 | Conectores (J5–J9) | unen los controles de la tapa con el shield |
| 4 | Tornillos M3 | cierran la tapa |

### Conexiones

| Pin Arduino | Componente | Conexión |
|---|---|---|
| `A0` | Slider 10 kΩ | cursor · extremos a 5V y GND |
| `A1` | Perilla 10 kΩ | pata central · extremos a 5V y GND |
| `D2` | Botón | a GND · usa `INPUT_PULLUP` (sin resistencia) |
| `D6` ~ | LED sol | ánodo con R1 de 220 Ω · cátodo a GND |
| `D9` ~ | LED del botón (opcional) | ánodo con R2 de 220 Ω · cátodo a GND |
| `5V` / `GND` | Rieles del shield | alimentan ambos potenciómetros |

---


## 📦 Carcasa

![Vista previa de la caja](docs/caja_vista_previa.png)

Una **cuña inclinada** de 120 × 120 mm (30 mm adelante, 65 mm atrás) impresa en 3D:

- **Base morada** con soportes para el Arduino UNO, 4 torres para tornillos M3, un orificio trasero para el cable USB y mini piezas del juego grabadas en los costados.
- **Tapa** de 3 mm con:
  - un **sol pixelado calado**, con franjas retro y cerros grabados debajo;
  - **etiquetas grabadas** en letra pixel: `MOVER`, `GIRAR`, `CAER` y `SUNSET BLOCKS`;
  - una pestaña que encaja en la base y guías para el cuerpo del slider.
- **Cámara del LED** en PLA blanco, con una cara de 0.6 mm que difumina la luz del sol.
- **Perilla pixelada** para el potenciómetro y **tapa de slider** con forma de bloque.

### Impresión (Bambu Lab)

| Pieza | Material / color | Notas |
|---|---|---|
| Base | PLA morado | sin soportes |
| Tapa | PLA morado + rosado | boca abajo; los grabados van en rosado |
| Cámara del LED | PLA blanco | el difusor va contra la cama |
| Perillas | PLA a elección | probar la ranura que calce (1.2 / 1.4 / 1.6 mm) |

Ajustes: capa de 0.2 mm, 3 perímetros, 15 % de relleno, sin soportes.

**Letras de color:**
- **Con AMS:** carga el 3MF como un solo objeto con partes y asigna rosado a `grabados_color`.
- **Sin AMS:** cambia el filamento a rosado a 0.6 mm y de vuelta a morado a 1.2 mm.

Todos los modelos son **paramétricos**: [`modelos/sunset_box.py`](modelos/sunset_box.py) regenera las piezas si cambias una medida (requiere `manifold3d`, `trimesh` y `rhino3dm`).

---

## 💻 Software

### Arduino → PC (protocolo serial)

El Arduino envía una línea cada **20 ms** a **115200 baudios**:

```
S,<slider>,<perilla>,<boton>,0
S,512,300,1,0
```

- `slider` y `perilla`: 0–1023, promediados en 8 lecturas para quitar ruido.
- `boton`: 1 = presionado, con antirrebote por software.

El juego le responde al Arduino con una letra para controlar las luces:

| Letra | Evento |
|---|---|
| `A` | pantalla de inicio |
| `S` | jugando |
| `K` | pieza fijada |
| `L` | línea completa |
| `T` | 4 líneas |
| `P` | pausa |
| `G` | game over |

### Dashboard web

[`web/index.html`](web/index.html) es un solo archivo HTML + JavaScript (canvas 256 × 224, estilo 16 bits) que se conecta al Arduino con la **Web Serial API**. Incluye:

- **Panel `CTRL` en tiempo real** con la posición del slider, el ángulo de la perilla, el estado del botón y el estado de la conexión USB.
- Sonidos chiptune generados con Web Audio.
- Récord guardado en el navegador.
- Fondo que cambia de la tarde a la noche según el nivel.
- **Modo teclado**, para probar sin Arduino: flechas para mover, Z/X para girar, espacio para caer, P para pausa y D para ver los valores crudos.

---

## ▶️ Cómo usarlo

1. Sube [`arduino/sunset_blocks_led/sunset_blocks_led.ino`](arduino/sunset_blocks_led/sunset_blocks_led.ino) al Arduino UNO con el Arduino IDE.
2. **Cierra el Monitor Serie**, porque solo un programa a la vez puede usar el puerto.
3. Abre `web/index.html` en **Google Chrome o Edge**. Web Serial no funciona en Safari ni en Firefox.
4. Haz clic en **CONECTAR ARDUINO** y elige el puerto (en Mac aparece como `cu.usbmodem…`).
5. Aprieta el botón para jugar.

**Problemas comunes**

- **El Arduino no aparece en la lista:** cierra el Monitor Serie y las otras pestañas del juego, y revisa que el cable USB transmita datos.
- **"Resource busy" al subir el código:** el juego tiene tomado el puerto. Cierra la pestaña y vuelve a subirlo.
- **La pieza se mueve al revés:** cambia `INVERT_SLIDER` o `INVERT_KNOB` a `true` al inicio de `index.html`.

---



## 📁 Estructura del repositorio

```
sunset-blocks/
├── README.md
├── arduino/
│   └── sunset_blocks_led/
│       └── sunset_blocks_led.ino   ← código del Arduino
├── web/
│   └── index.html                  ← juego + dashboard (Web Serial)
├── docs/
│   ├── esquema_sunset_blocks.png   ← diagrama esquemático del shield
│   ├── esquema_sunset_blocks.svg
│   ├── caja_vista_previa.png
│   ├── juego_titulo.png
│   └── juego_partida.png
├── modelos/
│   ├── 1_caja_y_tapa_bambu.3mf     ← base + tapa (Bambu Studio)
│   ├── 2_piezas_chicas_bambu.3mf   ← cámara LED, perillas, probeta
│   ├── sunset_blocks_caja.3dm      ← ensamble en Rhino
│   ├── sunset_box.py               ← modelo paramétrico
│   └── stl/                        ← todas las piezas en STL
└── fotos/
    ├── manufactura/
    ├── soldadura/
    └── ensamble/
```

---


