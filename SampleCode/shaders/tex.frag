# version 120 

// Mine is an old machine.  For version 130 or higher, do 
// in vec4 color ;  
// That is certainly more modern

varying vec4 color ;

uniform sampler2D tex ; 
uniform int istex ; 

void main (void) 
{       
        if (istex > 0) gl_FragColor = texture2D(tex, gl_TexCoord[0].st) ; 
//        else gl_FragColor = gl_Color ; 
        else gl_FragColor = color ; 
}
