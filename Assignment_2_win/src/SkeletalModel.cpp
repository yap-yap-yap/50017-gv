///=========================================================================================///
///
///                       Functions to be filled in for Assignment 2   
///
///           IMPORTANT: you ONLY need to work on functions with TODO in this section    
///    
///=========================================================================================///


#include "SkeletalModel.h"


///=========================================================================================///
///                                    Load .skel File    
///=========================================================================================///


// TODO: Load the skeleton from file here, create hierarchy of joints
//       (i.e., set values for m_rootJoint and m_joints)
void SkeletalModel::loadSkeleton(const char* filename)
{
    string line;
    string path = "data/" + std::string(filename);
    std::ifstream skelFile(path);
    while (getline(skelFile, line)) {
        // variables for iterating through file
        string value;
        std::stringstream linestream(line);
        int i = 0;
        // variables used to construct the Joints
        vector<float> transformVec;
        int parent_index;
        while (getline(linestream, value, ' ')) {
            if (i < 3) {
                transformVec.push_back(std::stof(value));
            }
            else if (i == 3) {
                parent_index = std::stol(value);
            }
            i++;
        }
        // construct translation matrix
        glm::mat4 transformMat = glm::mat4(
            1.0, 0.0, 0.0, transformVec[0],
            0.0, 1.0, 0.0, transformVec[1],
            0.0, 0.0, 1.0, transformVec[2],
            0.0, 0.0, 0.0, 1.0);
        //glm::mat4 transformMat(1.0f);
        //transformMat[3] = glm::vec4(glm::vec3(transformVec[0], transformVec[1], transformVec[2]), 1.0f);
        //std::cout << parent_index << endl;

        Joint* joint = new Joint;
        joint->transform = transformMat;

        if (parent_index == -1) {
            m_rootJoint = joint;
        }
        else if (parent_index >= 0) {
            // retrieve parent joint to add children. assumes that you will never encounter index out of order (child before parent)
            Joint* parentJoint = m_joints[parent_index];
            parentJoint->children.push_back(joint);
        }
        m_joints.push_back(joint);
    }

    //std::cout << m_joints.size() << endl;
}




///=========================================================================================///
///                         Compute transformations for Joints and Bones     
///=========================================================================================///

void SkeletalModel::computeTransforms()
{
    if (m_joints.size() == 0)
        return;

    computeJointTransforms();

    computeBoneTransforms();
}

// Compute a transformation matrix for each joint (i.e., ball) of the skeleton
void SkeletalModel::computeJointTransforms()
{
    jointMatList.clear();

    m_matrixStack.clear();

    computeJointTransforms(m_rootJoint, m_matrixStack);
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint* joint, MatrixStack matrixStack)
{
    matrixStack.push(joint->transform);

    if (joint->children.size() != 0) {
        for (int i = 0; i < joint->children.size(); i++) {
            computeJointTransforms(joint->children[i], matrixStack);
        }
    }
    jointMatList.push_back(matrixStack.top());
    matrixStack.pop();
}


// Compute a transformation matrix for each bone (i.e., cylinder) between each pair of joints in the skeleton
void SkeletalModel::computeBoneTransforms( )
{
    boneMatList.clear();

    m_matrixStack.clear();

    computeBoneTransforms(m_rootJoint, m_matrixStack);
}

// TODO: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the bones
// done using current joint to children (rather than current to parent) because that feels simpler, since you can access children for each joint and get their transformation
void SkeletalModel::computeBoneTransforms(Joint* joint, MatrixStack matrixStack)
{
    matrixStack.push(joint->transform);


    if (joint->children.size() != 0) {
        for (int i = 0; i < joint->children.size(); i++) {
            Joint* child_joint = joint->children[i];
            glm::vec3 child_vec = glm::vec3(child_joint->transform[0][3], child_joint->transform[1][3], child_joint->transform[2][3]); // for calculing distance from current joint to child joint
            
            glm::vec3 z_axis = glm::normalize(child_vec);
            glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, glm::vec3(0.0f, 0.0f, 1.0f)));
            glm::vec3 x_axis = glm::normalize(glm::cross(y_axis, z_axis));

            glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.5f));
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, glm::length(child_vec)));
            glm::mat4 rotate = glm::mat4(
                x_axis[0], x_axis[1], x_axis[2], 0.0f,
                y_axis[0], y_axis[1], y_axis[2], 0.0f,
                z_axis[0], z_axis[1], z_axis[2], 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );


            // push the transforms into the matrix stack to compute the full matrix via matrixStack.top()
            /*matrixStack.push(translate);
            matrixStack.push(scale);
            matrixStack.push(rotate);*/

            glm::mat4 transform = glm::transpose(rotate * scale * translate); // glm matrices are column first
            matrixStack.push(transform);
            boneMatList.push_back(matrixStack.top());
            matrixStack.pop();
            /*matrixStack.pop();
            matrixStack.pop();*/

            computeBoneTransforms(joint->children[i], matrixStack);
        }
    }

   
    matrixStack.pop();
}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// TODO: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    m_joints[jointIndex]->transform = m_joints[jointIndex]->transform * rotateX * rotateY * rotateZ;
}







