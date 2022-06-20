# Assignment 2

## Load .skel File

I iterated through each line in the file. For each line, I constructed the translation matrix for the joint, and added it to the joint object. I checked whether the joint is the root joint, and if it is, I searched through ``m_joints`` for its parent to add it to its children. This makes the assumption that no children will be ordered before its parent in the .skel file.

## Draw Joints

The transformation matrices that are pushed into the matrix stack are the necessary matrices for positioning the joints, thus no additional transformations are needed. The recursive function goes through all children and pushes all their transformation matrices to the stack, and the computed matrices are pushed into ``jointMatList``.

## Draw Bones

The bones are drawn between the current joint and its children joints. The transformations done onto each cylinder in order are: all previous parent transformations, translate (0.25, 0.25, 0.5), scale (0.01, 0.01, ``distance between current joint and child joint``), rotation. The rotation involves calculating cross products of the new axes after rotation, and populating the matrix with them as such:

```
x_axis[0], x_axis[1], x_axis[2], 0.0f,
y_axis[0], y_axis[1], y_axis[2], 0.0f,
z_axis[0], z_axis[1], z_axis[2], 0.0f,
     0.0f,      0.0f,      0.0f, 1.0f
```

## Set Joint Rotation

The transformation matrix will be applied to all children through the matrix stack, thus the only transformation that is necessary is to change the transformation matrix of the given joint. This is done by constructing the 3 Euler angle transformation matrices, and multiplying them together.