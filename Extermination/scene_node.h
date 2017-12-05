#ifndef SCENE_NODE_H_
#define SCENE_NODE_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "resource.h"


namespace game {
	class Camera;

    // Class that manages one object in a scene 
    class SceneNode {

        public:
            // Create scene node from given resources
			SceneNode(std::string name = std::string(""), Resource *geometry = NULL, Resource *material = NULL, Resource *texture = NULL);

            // Destructor
            ~SceneNode();
            
            // Get name of node
            const std::string GetName(void) const;
			void setName(std::string);

            // Get node attributes
            glm::vec3 GetPosition(void) const;
            glm::quat GetOrientation(void) const;
            glm::vec3 GetScale(void) const;

            // Set node attributes
            void SetPosition(glm::vec3 position);
            void SetOrientation(glm::quat orientation);
            void SetScale(glm::vec3 scale);
            
            // Perform transformations on node
            void Translate(glm::vec3 trans);
            void Rotate(glm::quat rot);
            void Scale(glm::vec3 scale);
			void addChild(SceneNode*);
			void setParent(SceneNode*);
			bool setGeometry(Resource*);
			bool setMaterial(Resource*);
			bool setTexture(Resource*);
			glm::vec3 getPos();
			void SetReset(float);
            void SetColor(glm::vec3 color);

            // Draw the node according to scene parameters in 'camera'
            // variable
            virtual void Draw(Camera *camera);

            // Update the node
            virtual void Update(double delta_time);
			virtual glm::mat4 getTransf();

            // OpenGL variables
            GLenum GetMode(void) const;
            GLuint GetArrayBuffer(void) const;
            GLuint GetElementArrayBuffer(void) const;
            GLsizei GetSize(void) const;
            GLuint GetMaterial(void) const;
			SceneNode *GetParent(void);
			virtual bool done();
			virtual float GetRadius();
			float GetExpDamage();
			void SetExpDamage(float);
			float GetRad();
			void RemoveChildren();

        protected:
            std::string name_; // Name of the scene node
            GLuint array_buffer_; // References to geometry: vertex and array buffers
            GLuint element_array_buffer_;
            GLenum mode_; // Type of geometry
            GLsizei size_; // Number of primitives in geometry
            GLuint material_; // Reference to shader program
            GLuint texture_; // Reference to texture resource
            glm::vec3 position_; // Position of node
            glm::quat orientation_; // Orientation of node
            glm::vec3 scale_; // Scale of node
			ResourceType geoType;
			float reset_;
			float start_time_;
			float curr_time_;
			glm::vec3 color_;
			float expDamage_;
			float rad_;

            // Set matrices that transform the node in a shader program
            void SetupShader(GLuint program);

			SceneNode *parent;
			std::vector<SceneNode*> children;

    }; // class SceneNode

} // namespace game

#endif // SCENE_NODE_H_
