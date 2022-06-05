# 50.0017: Assignment 1

## Mesh Loading

I first traversed the .obj file and picked out the relevant lines (lines starting with 'v ', 'vn' and 'f '), and concantenated them into separate strings ``vStr``, ``vnStr``, ``fStr``, separated by whitespace. For the face values, I changed the backslash to whitespace. Next, I converted these strings into vectors ``vList``, ``vnList`` and ``fList``. Finally, to construct ``verList`` and ``triList``, I traversed ``vList``, searched for the matching index in ``fList``, and found the matching normal to each vertex. Then I traversed ``fList`` and inserted them into ``triList``.

This method can be more efficient (lots of intermediate steps to split the information), but I implemented it in the way that made the most sense to me at first. One way to make it more efficient is to only traverse ``fList``, and extract the vertex with corresponding normal at each face value, and add it to ``verList`` if it is not already filled. This would need ``verList`` to be constructed as a fixed size structure first so that the correct index can be selected.

## Mesh Colouring

I added 1 to the ``colorID`` whenever ``SetMeshColor`` is called. To keep the ID value within the range of the color table, I took the modulus of ``colorID``.

## Mesh Transformation

For the rotation of the mesh, I used ``glm::rotate`` to rotate the mesh with the given axis and angle. For translation, I moved both ``camera_position`` and ``camera_target`` to move the camera independently from the mesh, along the plane of the display window. For scaling, I changed the z coordinate of ``camera_position`` in order to move the camera backwards and forwards from the mesh, effectively scaling it up and down. 