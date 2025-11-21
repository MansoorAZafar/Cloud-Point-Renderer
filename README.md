# Overview
- Simple OpenGL application to render a file with vertices and faces as a point cloud using baycentric's formula.
- Movement with WASD + Looking around with mouse / touchpad
- The repository comes with GLFW, GLAD and GLM already all set with CMake
- By default, running the application will use the resources/trophy.obj 

  
# Showcase
<img width="640" height="480" alt="trophy-image" src="https://github.com/user-attachments/assets/33044d3c-adbd-44be-a705-0a49880fddb0" />

<img width="640" height="480" alt="skull-image" src="https://github.com/user-attachments/assets/190fb9c4-65ae-4bda-ab66-9c5dd03989e0" />

<img width="640" height="480" alt="dragon-image" src="https://github.com/user-attachments/assets/5492544c-4802-4ccd-ba69-516d00380903" />

<img width="640" height="480" alt="cube-image" src="https://github.com/user-attachments/assets/39d5f346-4fd8-49a3-bd85-2e6dd57d83b8" />


# Controls

| **Key**    | **Description**      |
| ---------- | -------------------- | 
| W          | Move Forward         |
| A          | Move Left            |
| S          | Move Backwords       |
| D          | Move Right           |
| Escape     | Pause                |
| Escape + q | Quit the application |


# Installation

```
# 1. Clone the Repository
git clone https://github.com/MansoorAZafar/Cloud-Point-Renderer.git


# 2. Change into the repository
cd Cloud-Point-Renderer


# 3. Run run shell script
# IF on powershell (pwsh)
sh .\run.sh
# IF on bash
./run.sh


# 3.5 If you want to run with your own object
./run.sh [path-to-file].[extension]
# ex.
./run.sh resources/Cube.txt
```

# Rendering Your Own Objects 
- You can render other object as long as the file has verticies and faces
- the standard is blender's style, where all vertex lines are prefixed with 'v' and all face lines are prefixed with f
  - You don't need any other of the text other than just the v and f's
  - Example:
    ```txt
      v 1.000000 -1.000000 -1.000000
      v 1.000000 -1.000000 1.000000
      v -1.000000 -1.000000 1.000000
      v -1.000000 -1.000000 -1.000000
      v 1.000000 1.000000 -0.999999
      v 0.999999 1.000000 1.000001
      v -1.000000 1.000000 1.000000
      v -1.000000 1.000000 -1.000000

      f 2/1/1 3/2/1 4/3/1
      f 8/1/2 7/4/2 6/5/2
      f 5/6/3 6/7/3 2/8/3
      f 6/8/4 7/5/4 3/4/4
      f 3/9/5 7/10/5 8/11/5
      f 1/12/6 4/13/6 8/11/6
      f 1/4/1 2/1/1 4/3/1
      f 5/14/2 8/1/2 6/5/2
      f 1/12/3 5/6/3 2/8/3
      f 2/12/4 6/8/4 3/4/4
      f 4/13/5 3/9/5 8/11/5
      f 5/6/6 1/12/6 8/11/6
    ```
      - [result](https://github.com/user-attachments/assets/39d5f346-4fd8-49a3-bd85-2e6dd57d83b8)
    - Then just follow step 3.5 from [Installation](#Installation)
        

# Upcoming Features (ordered by priority)
1. A loading screen for objects that take a long time to load
2. A button to load objects rather than re-opening the project and needing to pass an argument
3. A way to increase player speed via scroll wheel
4. Integration with GitHub pages for a way to interact with it via web rather than downloading
5. Package manager for CMake rather than embedding dependencies into project (... maybe, I like out-of-the-box applications)
