# version 120 

/* This is the fragment shader for reading in a scene description, including 
   lighting.  Uniform lights are specified from the main program, and used in 
   the shader.  As well as the material parameters of the object.  */

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color;  
// in vec3 mynormal; 
// in vec4 myvertex;
// That is certainly more modern

varying vec4 color;
varying vec3 mynormal; 
varying vec4 myvertex; 

const int numLights = 10; 
uniform bool enablelighting; // are we lighting at all (global).
uniform vec4 lightposn[numLights]; // positions of lights 
uniform vec4 lightcolor[numLights]; // colors of lights
uniform int numused;               // number of lights used

// Now, set the material parameters.  These could be varying and/or bound to 
// a buffer.  But for now, I'll just make them uniform.  
// I use ambient, diffuse, specular, shininess as in OpenGL.  
// But, the ambient is just additive and doesn't multiply the lights.  

uniform vec4 ambient; 
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess; 

vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) {

        //dot product of the normal and the direction of light
        float nDotL = dot(normal, direction)  ;
        vec4 lambert = mydiffuse * lightcolor * max (nDotL, 0.0) ;  //note that we don't consider any attenuation

        float nDotH = dot(normal, halfvec) ;//Blinn-Phong formula..halfvec is the half normalized vector between light source and the eye 
        vec4 phong = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; //note that we don't consider any attenuation
                
        vec4 retval = lambert + phong ; 
        return retval ;
}

void main (void) 
{       
    if (enablelighting) {       
        vec4 finalcolor; 

        // YOUR CODE FOR HW 2 HERE
        // A key part is implementation of the fragment shader

	const vec3 eyepos = vec3(0,0,0);

	vec4 _mypos = gl_ModelViewMatrix * myvertex;
	vec3 mypos  = _mypos.xyz /_mypos.w;

	vec3 eyedirn = normalize(eyepos - mypos);
	vec3 normal = normalize(gl_NormalMatrix * mynormal);
	
	vec4 accumulator = vec4(0,0,0,0);
	for (int i=0; i< numused; i++){
		vec3 direction, position;
		vec4 lc;

		switch(i){
			case 0:
				 if (lightposn[0].w == 0){
					direction = normalize(lightposn[0].xyz);
				}
				else {  
					position = lightposn[0].xyz / lightposn[0].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[0];
				break;
			case 1:
				 if (lightposn[1].w == 0){
					direction = normalize(lightposn[1].xyz);
				}
				else {  
					position = lightposn[1].xyz / lightposn[1].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[1];
				break;
			case 2:
				 if (lightposn[2].w == 0){
					direction = normalize(lightposn[2].xyz);
				}
				else {  
					position = lightposn[2].xyz / lightposn[2].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[2];
				break;
			case 3:
				 if (lightposn[3].w == 0){
					direction = normalize(lightposn[3].xyz);
				}
				else {  
					position = lightposn[3].xyz / lightposn[3].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[3];
				break;
			case 4:
				 if (lightposn[4].w == 0){
					direction = normalize(lightposn[4].xyz);
				}
				else {  
					position = lightposn[4].xyz / lightposn[4].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[4];
				break;
			case 5:
				 if (lightposn[5].w == 0){
					direction = normalize(lightposn[5].xyz);
				}
				else {  
					position = lightposn[5].xyz / lightposn[5].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[5];
				break;
			case 6:
				 if (lightposn[6].w == 0){
					direction = normalize(lightposn[6].xyz);
				}
				else {  
					position = lightposn[6].xyz / lightposn[6].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[6];
				break;
			case 7:
				 if (lightposn[7].w == 0){
					direction = normalize(lightposn[7].xyz);
				}
				else {  
					position = lightposn[7].xyz / lightposn[7].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[7];
				break;
			case 8:
				 if (lightposn[8].w == 0){
					direction = normalize(lightposn[8].xyz);
				}
				else {  
					position = lightposn[8].xyz / lightposn[8].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[8];
				break;
			case 9:
				 if (lightposn[9].w == 0){
					direction = normalize(lightposn[9].xyz);
				}
				else {  
					position = lightposn[9].xyz / lightposn[9].w;
					direction = normalize(position - mypos);
				
				}
				lc = lightcolor[9];
				break;


		}
/*		if (lightposn[i].w == 0){
			direction = normalize(lightposn[i].xyz);
		}
		else {
			position = lightposn[i].xyz / lightposn[i].w;
			direction = normalize(position - mypos);

		}*/
		vec3 half = normalize(direction + eyedirn);
		
		//vec4 colApp = ComputeLight(direction, lightcolor[i], normal, half, diffuse, specular, shininess);
		vec4 colApp = ComputeLight(direction, lc, normal, half, diffuse, specular, shininess);
		accumulator += colApp;

	}


        // Color all pixels blue for now, remove this in your implementation!
        finalcolor = vec4(0,0,1,1); 


        gl_FragColor =  ambient + emission + accumulator;;
    } else {
        gl_FragColor = color; 
    }
}
