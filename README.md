# Real-Time Water & Weather Simulation | UE5

An interactive Unreal Engine 5.6 visualization prototype for exploring how changing weather, time of day, and ocean-wave parameters affect the appearance of a coastal scene. The project combines a fly-through camera, an on-screen control panel, dynamic sky and weather, a configurable ocean surface, and streamed geospatial terrain and buildings.

This is a **visualization and interaction project**, not a scientifically validated weather forecast or fluid-dynamics simulation.

## Project gallery

The gallery leads with weather, then shows wave response, the complete interface, and time-of-day lighting. Capture the **running application**, not the Unreal Editor. Save PNG screenshots in `docs/images/` with the exact filenames below. The image links will appear after you add those files to the repository.

For meaningful comparisons, keep the camera framing and resolution the same within each group. Wait for Cesium terrain and buildings to finish loading, hide the panel for scene comparisons, and avoid debug messages or performance warnings. A 16:9 frame is recommended.

### Weather and scene

`01-clear-weather.png` — Main portfolio image: a wide, unobstructed view of the coast, ocean, sky, terrain, and buildings in clear weather.

![Coastal scene in clear weather](docs/images/01-clear-weather.png)

`02-rainy-weather.png` and `03-alternate-weather.png` — Use the **same camera angle and time of day** as the main image. For the third image, choose another available preset with a visibly different atmosphere, such as overcast or stormy weather.

| Rainy weather | Another weather condition |
| --- | --- |
| ![Same coastal scene in rainy weather](docs/images/02-rainy-weather.png) | ![Same coastal scene in another weather condition](docs/images/03-alternate-weather.png) |

### Ocean-wave response

`04-calm-waves.png` and `05-strong-waves.png` — Frame the water from the same camera position, under the same weather and lighting. Show a restrained wave setting first, then a clearly stronger setting after pressing **Apply**.

| Calm waves | Stronger waves |
| --- | --- |
| ![Ocean with restrained waves](docs/images/04-calm-waves.png) | ![Ocean with stronger waves](docs/images/05-strong-waves.png) |

### Interface and controls

`06-control-panel.png` — Show the open control panel clearly enough to read the time and weather controls, ocean-wave sliders, **Apply**, the **Camera Speed** slider, and the show/hide, **Settings**, and **Exit** controls. If everything does not fit legibly in one frame, prioritize a readable view rather than shrinking the image.

![Full weather, ocean, and camera control panel](docs/images/06-control-panel.png)

`07-settings-panel.png` — Open **Settings** and capture the options shown there. Together, these two images document the available UI functions.

![Application settings panel](docs/images/07-settings-panel.png)

### Time-of-day lighting

`08-morning-light.png` and `09-late-afternoon-light.png` — Use the same camera angle, weather, and wave settings. Include terrain or buildings that cast visible shadows, so the change in sun position and lighting is clear rather than showing only a different sky color.

| Morning | Late afternoon |
| --- | --- |
| ![Morning sun position and shadows](docs/images/08-morning-light.png) | ![Late-afternoon sun position and shadows](docs/images/09-late-afternoon-light.png) |

## What the project demonstrates

- **Real-time environmental changes:** adjust the time of day and choose a weather condition while viewing the scene.
- **Interactive ocean controls:** change water height, wave blend duration, wave direction on the X and Y axes, and wave strength, then apply the settings.
- **Explorable scene:** fly through the environment with a keyboard and mouse.
- **Adjustable camera speed:** use the camera-speed slider to move from the original speed (1×) up to ten times that speed (10×).
- **Packaged Windows build:** run the demonstration without opening the Unreal Editor.

The project-specific work focuses on bringing these systems together into a usable real-time experience, including the control-panel workflow, fly-camera behavior, speed adjustment, and Windows packaging. The underlying weather, water, and geospatial systems are credited below; they are not presented as original implementations.

## Controls

| Action | Input |
| --- | --- |
| Move forward / backward | `W` / `S` |
| Move left / right | `A` / `D` |
| Move down / up | `Q` / `E` |
| Look around | Hold the **right mouse button** and move the mouse |
| Interact with the control panel | Release the right mouse button to use the cursor |

In the **Weather and Time** section, use the time controls and weather selector to change the atmosphere. In **Ocean Waves**, move the sliders for water height, wave blend duration, X/Y wave direction, and wave strength, then select **Apply**. The **Camera Speed** slider starts at the project's original movement speed and increases it up to 10×. Use the panel's show/hide control when you want an unobstructed view; **Exit** closes the application.

The Date and Location controls from the underlying weather widget are intentionally hidden in this project's interface. Their absence from the panel does not mean the weather system has no internal date or location state.

## Run the Windows build

1. Download the Windows build ZIP from this repository's **Releases** section, if a release has been published.
2. Extract the **entire** archive to a folder. Keep the executable and its accompanying folders together.
3. Run `WaterSimTest.exe` inside the extracted `Windows` folder.

A 64-bit Windows PC with a GPU capable of running an Unreal Engine 5.6 scene is required. Performance depends on the hardware, display resolution, and graphics settings. Cesium terrain and building tiles may need an internet connection and valid service credentials; if they cannot load, the geospatial part of the scene may appear incomplete. The packaged application is a folder-based build, **not** a standalone `.exe` that can be moved by itself.

## Source code and repository scope

This public portfolio repository contains the project documentation, the screenshot gallery, and the original **FlyViewer** C++ plugin source. The plugin implements the fly-camera movement, mouse-look interaction, and 1×–10× camera-speed control. Its source is under `Plugins/FlyControllerPlugins/Source/FlyViewer/`, with the plugin descriptor at `Plugins/FlyControllerPlugins/FlyViewer.uplugin`.

The full Unreal Engine 5.6 editor project is **not included**. In particular, this repository omits the level, Blueprint widgets, licensed Fluid Flux and Ultra Dynamic Sky assets, Cesium-related data and credentials, and generated build files. Therefore, cloning the repository alone will **not** open or rebuild the complete scene. Use the packaged Windows release to experience the finished application, if one has been published. **The Release provides a runnable application, not the complete editable Unreal Engine project.**

## Notes for reviewers

- The weather and wave controls are intended to make visual changes easy to observe and compare; their values should not be interpreted as measured real-world conditions.
- Cesium content is streamed, so first load times and the appearance of terrain/buildings can depend on network access and service availability.
- The application may be demanding on lower-end hardware. Reduce Unreal graphics settings or resolution if frame rate is low.
- Screenshots or video captured from one machine may not exactly match another machine's performance or streamed geospatial detail.

## Third-party technology and asset notice

This project uses **Unreal Engine 5**, **Cesium for Unreal**, **Fluid Flux**, and **Ultra Dynamic Sky**. Their code, assets, trademarks, and distribution rights remain subject to their owners' terms. This README does not claim that those third-party components are original work or freely redistributable source assets. Before publishing a packaged release, check the applicable licenses and remove private tokens, caches, and machine-specific build files.
