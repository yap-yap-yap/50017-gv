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
void SkeletalModel::computeBoneTransforms(Joint* joint, MatrixStack matrixStack)
{
  
}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// TODO: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{

}







