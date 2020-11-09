xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 121;
 1.73174;-1.09350;-2.27154;,
 1.73174;-1.09350;3.07846;,
 1.73174;-2.86387;3.07846;,
 1.73174;-2.08680;-2.27154;,
 1.73174;-1.09350;3.07846;,
 -1.40826;-1.09350;3.07846;,
 -1.40826;-2.08680;3.07846;,
 1.73174;-2.86387;3.07846;,
 -2.26805;-1.09350;-2.27154;,
 -2.26805;-1.09350;3.07846;,
 -1.40826;-1.09350;3.07846;,
 -1.40826;-1.09350;-2.27154;,
 -2.26805;-2.08680;-2.27154;,
 -1.40826;-2.08680;-2.27154;,
 -2.26805;-2.08680;3.07846;,
 -1.40826;-2.08680;3.07846;,
 1.73174;-0.01876;3.07846;,
 -1.40826;-0.01876;3.07846;,
 1.73174;-0.01876;-2.27154;,
 1.73174;-1.09350;-2.27154;,
 -1.40826;-0.01876;-2.27154;,
 -1.40826;-1.09350;-2.27154;,
 1.73174;-3.72366;3.07846;,
 1.73174;-3.72366;-2.27154;,
 1.73174;-2.08680;-2.27154;,
 -1.40826;-3.72366;-2.27154;,
 -1.40826;-3.72366;3.07846;,
 -2.26805;-3.72366;3.07846;,
 -2.26805;-3.72366;-2.27154;,
 -1.40826;-3.72366;-2.27154;,
 1.73174;-1.09350;-2.21013;,
 -1.40826;-1.09350;-2.21013;,
 -1.40826;-1.09350;-2.27154;,
 1.73174;-1.09350;-2.27154;,
 1.73174;-2.08680;-2.21013;,
 1.73174;-2.08680;-2.27154;,
 -1.40826;-2.08680;-2.21013;,
 -1.40826;-2.08680;-2.27154;,
 -1.40826;-3.72366;-3.03927;,
 1.73174;-3.72366;-3.03927;,
 1.73174;-2.08680;-3.03927;,
 -1.40826;-2.08680;-3.03927;,
 -1.40826;-2.08680;-2.27154;,
 1.73174;-3.72366;3.84619;,
 -1.40826;-3.72366;3.84619;,
 1.73174;-2.86387;3.84619;,
 -1.40826;-2.08680;3.84619;,
 -2.26805;-3.72366;-3.03927;,
 -1.40826;-3.72366;-3.03927;,
 -1.40826;-2.08680;-3.03927;,
 -2.26805;-2.08680;-3.03927;,
 -2.26805;-3.72366;3.84619;,
 -2.26805;-2.08680;3.84619;,
 -2.85588;-3.72366;3.07846;,
 -2.85588;-3.72366;-2.27154;,
 -2.85588;-2.08680;-2.27154;,
 -2.85588;-2.08680;3.07846;,
 -2.85588;-3.72366;-3.03927;,
 -2.85588;-2.08680;-3.03927;,
 -2.85588;-2.08680;3.84619;,
 -2.85588;-3.72366;3.84619;,
 -0.98926;-3.21650;-3.79954;,
 0.67074;-3.21650;-3.79954;,
 0.67074;-6.37650;-3.79954;,
 -0.98926;-6.37650;-3.79954;,
 0.67074;-3.21650;-3.79954;,
 0.67074;-3.21650;-2.46954;,
 0.67074;-6.37650;-2.46954;,
 0.67074;-6.37650;-3.79954;,
 0.67074;-3.21650;-2.46954;,
 -0.98926;-3.21650;-2.46954;,
 -0.98926;-6.37650;-2.46954;,
 0.67074;-6.37650;-2.46954;,
 -0.98926;-3.21650;-2.46954;,
 -0.98926;-3.21650;-3.79954;,
 -0.98926;-6.37650;-3.79954;,
 -0.98926;-6.37650;-2.46954;,
 0.67074;-3.21650;-3.79954;,
 -0.98926;-3.21650;-3.79954;,
 -0.98926;-6.37650;-3.79954;,
 0.67074;-6.37650;-3.79954;,
 -2.90726;-3.21650;-2.69754;,
 -1.72726;-3.21650;-2.69754;,
 -1.72726;-6.37650;-2.69754;,
 -2.90726;-6.37650;-2.69754;,
 -1.72726;-3.21650;-2.69754;,
 -1.72726;-3.21650;-1.36754;,
 -1.72726;-6.37650;-1.36754;,
 -1.72726;-6.37650;-2.69754;,
 -1.72726;-3.21650;-1.36754;,
 -2.90726;-3.21650;-1.36754;,
 -2.90726;-6.37650;-1.36754;,
 -1.72726;-6.37650;-1.36754;,
 -2.90726;-3.21650;-1.36754;,
 -2.90726;-3.21650;-2.69754;,
 -2.90726;-6.37650;-2.69754;,
 -2.90726;-6.37650;-1.36754;,
 -1.72726;-3.21650;-2.69754;,
 -2.90726;-3.21650;-2.69754;,
 -2.90726;-6.37650;-2.69754;,
 -1.72726;-6.37650;-2.69754;,
 -2.90726;-3.21650;-0.41854;,
 -1.72726;-3.21650;-0.41854;,
 -1.72726;-6.37650;-0.41854;,
 -2.90726;-6.37650;-0.41854;,
 -1.72726;-3.21650;-0.41854;,
 -1.72726;-3.21650;3.82146;,
 -1.72726;-6.37650;3.82146;,
 -1.72726;-6.37650;-0.41854;,
 -1.72726;-3.21650;3.82146;,
 -2.90726;-3.21650;3.82146;,
 -2.90726;-6.37650;3.82146;,
 -1.72726;-6.37650;3.82146;,
 -2.90726;-3.21650;3.82146;,
 -2.90726;-3.21650;-0.41854;,
 -2.90726;-6.37650;-0.41854;,
 -2.90726;-6.37650;3.82146;,
 -1.72726;-3.21650;-0.41854;,
 -2.90726;-3.21650;-0.41854;,
 -2.90726;-6.37650;-0.41854;,
 -1.72726;-6.37650;-0.41854;;
 
 62;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,8,11,13;,
 4;9,14,15,10;,
 4;9,8,12,14;,
 4;16,17,10,1;,
 4;18,16,1,19;,
 4;20,18,19,21;,
 4;17,20,21,10;,
 4;17,16,18,20;,
 4;22,23,24,2;,
 4;25,23,22,26;,
 4;27,28,29,26;,
 4;30,31,32,33;,
 4;34,30,33,35;,
 4;36,34,35,37;,
 4;31,36,37,32;,
 4;31,30,34,36;,
 4;38,39,23,25;,
 4;40,41,42,24;,
 4;39,40,24,23;,
 4;39,38,41,40;,
 4;43,44,26,22;,
 4;45,43,22,2;,
 4;46,45,2,15;,
 4;44,43,45,46;,
 4;47,48,29,28;,
 4;49,50,12,13;,
 4;48,47,50,49;,
 4;44,51,27,26;,
 4;52,46,15,14;,
 4;51,44,46,52;,
 4;53,54,28,27;,
 4;55,56,14,12;,
 4;54,53,56,55;,
 4;57,58,50,47;,
 4;54,57,47,28;,
 4;58,55,12,50;,
 4;58,57,54,55;,
 4;59,60,51,52;,
 4;56,59,52,14;,
 4;60,53,27,51;,
 4;60,59,56,53;,
 4;61,62,63,64;,
 4;65,66,67,68;,
 4;69,70,71,72;,
 4;73,74,75,76;,
 4;73,66,77,78;,
 4;79,80,67,76;,
 4;81,82,83,84;,
 4;85,86,87,88;,
 4;89,90,91,92;,
 4;93,94,95,96;,
 4;93,86,97,98;,
 4;99,100,87,96;,
 4;101,102,103,104;,
 4;105,106,107,108;,
 4;109,110,111,112;,
 4;113,114,115,116;,
 4;113,106,117,118;,
 4;119,120,107,116;;
 
 MeshMaterialList {
  17;
  62;
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.702400;0.103200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.090000;0.079020;0.011610;;
  }
  Material {
   0.800000;0.636800;0.458400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.310000;0.246760;0.177630;;
  }
  Material {
   0.060000;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.037600;0.150400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.012800;0.060000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.344800;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.091200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.150000;0.150000;0.150000;;
  }
  Material {
   0.172800;0.062400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.800000;0.612000;1.000000;;
   5.000000;
   0.260000;0.260000;0.260000;;
   0.000000;0.250000;0.191250;;
  }
  Material {
   0.000000;0.380000;0.385600;1.000000;;
   16.000000;
   0.160000;0.160000;0.160000;;
   0.000000;0.152000;0.154240;;
  }
  Material {
   0.467200;0.452000;0.037600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.110000;0.110000;0.110000;;
  }
  Material {
   0.800000;0.398400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  8;
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.121003;0.992652;0.000000;,
  0.240228;0.970717;0.000000;;
  62;
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;6,7,7,6;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;0,6,6,0;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  121;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
