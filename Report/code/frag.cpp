vec4 textureColor = texture(s,tc);
color = textureColor*vec4(ambient + diffuse + specular, 1.0);   