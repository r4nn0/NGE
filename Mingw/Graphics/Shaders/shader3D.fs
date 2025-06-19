#version 460 core

out vec4 FragColor;

in DATA{
    vec3 FragPos;
    vec3 vNormals;
    vec4 color;
    vec2 texCoords;
    float textureSlot;
}fs_in;

uniform sampler2D texture[32];
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
vec4 base_color;


void main() {
    
    if(fs_in.textureSlot>=0){
        // base_color=vec4(texture2D(texture[int(fs_in.textureSlot)], fs_in.texCoords).rgb,1.0);
        FragColor = fs_in.color * texture2D(texture[int(fs_in.textureSlot)], fs_in.texCoords);
	}
	else
        FragColor = fs_in.color;
        
    /*
    // Ambient component
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse component
    vec3 norm = normalize(fs_in.vNormals);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular component
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // shininess
    vec3 specular = specularStrength * spec * lightColor;

    // Combine all components
    vec3 lighting = (ambient + diffuse + specular);

    FragColor = fs_in.color;
    //FragColor = vec4(lighting, 1.0)* fs_in.color;
	// FragColor = vec4(0,0,0,1);
    */
}
