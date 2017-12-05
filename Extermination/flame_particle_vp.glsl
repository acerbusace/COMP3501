#version 400

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 normal_mat;
uniform float timer;
uniform float reset;
uniform vec3 uniColor;

// Attributes forwarded to the geometry shader
out vec3 vertex_color;
out float timestep;
out vec4 particle_color;

// Simulation parameters (constants)
uniform vec3 up_vec = vec3(0.0, 1.0, 0.0);
uniform vec3 object_color = vec3(0.8, 0.8, 0.8);
//float grav = 0.01; // Gravity
float grav = 2.02; // Gravity
float speed = 2.5; // Allows to control the speed of the explosion

float dist = 1.75;
float r_time = 2;

// Define some useful constants
const float pi = 3.1415926536;
const float pi_over_two = 1.5707963268;
const float two_pi = 2.0*pi;

float sign(float num) {
	return (num < 0) ? -1.0 : 1.0;
}


void main()
{
    // Define particle id
    float particle_id = color.r; // Derived from the particle color. We use the id to keep track of particles
    float phase = two_pi*particle_id;
    float circtime = mod((timer / 0.5 + phase), r_time); // A cycle lasts 16 seconds
    //float circtime = mod((timer / 0.075 + phase), 16.0); // A cycle lasts 16 seconds
    //float circtime = mod((timer / reset + phase), 16.0); // A cycle lasts 16 seconds

    // Let's first work in model space (apply only world matrix)
    vec4 position = world_mat * vec4(vertex, 1.0);
    vec4 norm = normal_mat * vec4(normal, 1.0);

	vec3 norm3 = vec3(norm.x, norm.y, norm.z);
	float time_to_dist = dist / length(norm3);


    // Let time cycle every four seconds
    //float circtime = timer - 4.0 * floor(timer / 4);
    //float circtime = timer - reset * floor(timer / reset);
    //float circtime = timer - time_to_dist * floor(timer / time_to_dist);
    float t = circtime; // Our time parameter
    //float t = timer; // Our time parameter
    

    // Move point along normal and down with t*t (acceleration under gravity)
    //position.x += norm.x*t*speed - grav*speed*up_vec.x*t*t;
    //position.y += norm.y*t*speed - grav*speed*up_vec.y*t*t;
    //position.z += norm.z*t*speed - grav*speed*up_vec.z*t*t;

	//float tt = t / time_to_dist;
	float tt = t / r_time;
	//tt = (tt > 0.95) ? (tt - 0.95) / 0.05 : 0;
	tt = 0;
    position.x += norm.x*t*speed + grav*speed*up_vec.x*tt*tt;
    position.y += norm.y*t*speed + grav*speed*up_vec.y*tt*tt;
    position.z += norm.z*t*speed + grav*speed*up_vec.z*tt*tt;

    //position.x += ((norm.x*t*speed) - ((sign(norm.x)*dist) * floor((norm.x*t*speed) / (sign(norm.x)*dist))));
    //position.y += ((norm.y*t*speed) - ((sign(norm.y)*dist) * floor((norm.y*t*speed) / (sign(norm.y)*dist))));
    //position.z += ((norm.z*t*speed) - ((sign(norm.z)*dist) * floor((norm.z*t*speed) / (sign(norm.z)*dist))));
    
    // Now apply view transformation
    gl_Position = view_mat * position;
        
    // Define outputs
    // Define color of vertex
    //vertex_color = color.rgb; // Color defined during the construction of the particles
    //vertex_color = object_color; // Uniform color 
    //vertex_color = color; // Uniform color 
    //vertex_color = vec3(uniColor, uniColor, uniColor); // Uniform color 
    vertex_color = uniColor; // Uniform color 
    vertex_color = vec3(0, 1 - (circtime / time_to_dist), 1 - (circtime / time_to_dist)); // Uniform color 
	//float blend = 1 - circtime / time_to_dist;
	float blend = 1 - circtime / r_time;
    //particle_color = vec4(blend, blend, blend, 1 - alpa*alpa);
	vec3 b_color = vec3(1, 0, 0);
	vec3 e_color = vec3(0, 0, 1);
    particle_color = vec4(b_color.x + (e_color.x - b_color.x)*blend, b_color.y + (e_color.y - b_color.y)*blend, b_color.z + (e_color.z - b_color.z)*blend, 1);
	blend = (blend > 0.25) ? 1.0 : blend/0.25;
	//particle_color.x *= blend;
	//particle_color.y *= blend;
	//particle_color.z *= blend;
    //particle_color = vec4(blend*0.8, blend*0.4, blend*0.03, 1);
    //vertex_color = vec3(t, 0.0, 1-t);
    //vertex_color = vec3(1.0, 1-t, 0.0);

    // Forward time step to geometry shader
    timestep = t;
}
