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
 126;
 -2.73585;-0.54874;-2.30292;,
 -2.73585;-0.54874;0.55092;,
 -2.73585;0.03472;0.55092;,
 -2.73585;0.03472;-2.30292;,
 2.94016;-0.54874;0.55092;,
 2.94016;-0.54874;-2.30292;,
 2.94016;0.03472;-2.30292;,
 2.94016;0.03472;0.55092;,
 -2.73585;-0.54874;1.43460;,
 -2.73585;0.03472;1.43460;,
 -2.73585;-0.54874;-3.18660;,
 -2.73585;0.03472;-3.18660;,
 2.94016;-0.54874;1.43460;,
 2.94016;0.03472;1.43460;,
 2.94016;-0.54874;-3.18660;,
 2.94016;0.03472;-3.18660;,
 -2.73585;-0.54874;2.39861;,
 -2.73585;0.03472;2.39861;,
 -2.73585;-0.54874;-4.15061;,
 -2.73585;0.03472;-4.15061;,
 1.77533;-0.54874;2.39861;,
 2.94016;-0.54874;2.39861;,
 2.94016;0.03472;2.39861;,
 1.77533;0.03472;2.39861;,
 2.94016;-0.54874;-4.15061;,
 1.77533;-0.54874;-4.15061;,
 1.77533;0.03472;-4.15061;,
 2.94016;0.03472;-4.15061;,
 -2.73585;-0.54874;3.50321;,
 0.65067;-0.54874;3.50321;,
 0.65067;0.03472;3.50321;,
 -2.73585;0.03472;3.50321;,
 -2.73585;-0.54874;-5.25521;,
 -2.73585;0.03472;-5.25521;,
 1.77533;-0.54874;3.50321;,
 1.77533;0.03472;3.50321;,
 1.77533;-0.54874;-5.25521;,
 0.65067;-0.54874;-5.25521;,
 0.65067;0.03472;-5.25521;,
 1.77533;0.03472;-5.25521;,
 -2.73585;-1.93064;-2.30292;,
 -2.73585;-1.93064;0.55092;,
 -2.73585;-1.93064;1.43460;,
 -2.73585;-1.93064;-3.18660;,
 2.94016;-1.93064;1.43460;,
 2.94016;-1.93064;0.55092;,
 -2.73585;-1.93064;2.39861;,
 -2.73585;-1.93064;-4.15061;,
 1.77533;-1.93064;2.39861;,
 2.94016;-1.93064;2.39861;,
 2.94016;-1.93064;-3.18660;,
 2.94016;-1.93064;-4.15061;,
 -2.73585;-1.93064;3.50321;,
 0.65067;-1.93064;3.50321;,
 -2.73585;-1.93064;-5.25521;,
 1.77533;-1.93064;3.50321;,
 1.77533;-1.93064;-4.15061;,
 1.77533;-1.93064;-5.25521;,
 -2.73585;-2.51411;-2.30292;,
 -2.73585;-2.51411;0.55092;,
 0.65067;-2.51411;-2.30292;,
 0.65067;-2.51411;0.55092;,
 1.77533;-2.51411;-2.30292;,
 1.77533;-2.51411;0.55092;,
 2.94016;-2.51411;-2.30292;,
 2.94016;-2.51411;0.55092;,
 -2.73585;-2.51411;1.43460;,
 0.65067;-2.51411;1.43460;,
 -2.73585;-2.51411;-3.18660;,
 0.65067;-2.51411;-3.18660;,
 1.77533;-2.51411;1.43460;,
 1.77533;-2.51411;-3.18660;,
 2.94016;-2.51411;1.43460;,
 2.94016;-2.51411;-3.18660;,
 -2.73585;-2.51411;2.39861;,
 0.65067;-2.51411;2.39861;,
 -2.73585;-2.51411;-4.15061;,
 0.65067;-2.51411;-4.15061;,
 1.77533;-2.51411;2.39861;,
 1.77533;-2.51411;-4.15061;,
 2.94016;-2.51411;2.39861;,
 2.94016;-2.51411;-4.15061;,
 -2.73585;-2.51411;3.50321;,
 0.65067;-2.51411;3.50321;,
 -2.73585;-2.51411;-5.25521;,
 0.65067;-2.51411;-5.25521;,
 1.77533;-2.51411;3.50321;,
 1.77533;-2.51411;-5.25521;,
 1.77533;-2.51411;-7.30563;,
 2.94016;-2.51411;-7.30563;,
 1.77533;-1.93064;-7.30563;,
 2.94016;-1.93064;-7.30563;,
 -2.73585;-2.51411;-7.31272;,
 0.65067;-2.51411;-7.31272;,
 -2.73585;-1.93064;-7.31272;,
 0.65067;-1.93064;-7.31272;,
 0.65067;-1.93064;-5.25521;,
 1.77533;-2.51411;-7.31272;,
 1.77533;-1.93064;-7.31272;,
 1.77533;-1.93064;-6.31210;,
 2.94016;-1.93064;-6.31210;,
 2.94016;-0.54874;-6.31210;,
 1.77533;-0.54874;-6.31210;,
 -2.73585;-1.93064;-6.31084;,
 0.65067;-1.93064;-6.31084;,
 -2.73585;-0.54874;-6.31084;,
 0.65067;-0.54874;-6.31084;,
 1.77533;-1.93064;-6.31084;,
 1.77533;-0.54874;-6.31084;,
 -2.73585;0.03472;-5.96152;,
 -2.73585;-0.54874;-5.96152;,
 0.65067;0.03472;-5.96152;,
 0.65067;-0.54874;-5.96152;,
 2.96459;-0.54874;0.55092;,
 2.96459;-0.54874;-2.30292;,
 2.96459;-1.93064;0.55092;,
 2.96459;-1.93064;-2.30292;,
 2.96459;-0.54874;-3.18660;,
 2.96459;-1.93064;-3.18660;,
 2.96459;-2.51411;-2.30292;,
 2.96459;-2.51411;0.55092;,
 2.96459;-2.51411;1.43460;,
 2.96459;-1.93064;1.43460;,
 2.96459;-2.51411;-3.18660;,
 2.96459;-2.51411;-4.15061;,
 2.96459;-1.93064;-4.15061;;
 
 114;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;1,8,9,2;,
 4;10,0,3,11;,
 4;12,4,7,13;,
 4;5,14,15,6;,
 4;8,16,17,9;,
 4;18,10,11,19;,
 4;20,21,22,23;,
 4;21,12,13,22;,
 4;24,25,26,27;,
 4;14,24,27,15;,
 4;28,29,30,31;,
 4;16,28,31,17;,
 4;32,18,19,33;,
 4;29,34,35,30;,
 4;34,20,23,35;,
 4;36,37,38,39;,
 4;25,36,39,26;,
 4;40,41,1,0;,
 4;41,42,8,1;,
 4;43,40,0,10;,
 4;44,45,4,12;,
 4;42,46,16,8;,
 4;47,43,10,18;,
 4;48,49,21,20;,
 4;49,44,12,21;,
 4;50,51,24,14;,
 4;52,53,29,28;,
 4;46,52,28,16;,
 4;54,47,18,32;,
 4;53,55,34,29;,
 4;55,48,20,34;,
 4;56,57,36,25;,
 4;58,59,41,40;,
 4;60,61,59,58;,
 4;62,63,61,60;,
 4;64,65,63,62;,
 4;59,66,42,41;,
 4;67,66,59,61;,
 4;68,58,40,43;,
 4;68,69,60,58;,
 4;70,67,61,63;,
 4;69,71,62,60;,
 4;72,70,63,65;,
 4;71,73,64,62;,
 4;66,74,46,42;,
 4;75,74,66,67;,
 4;76,68,43,47;,
 4;76,77,69,68;,
 4;78,75,67,70;,
 4;77,79,71,69;,
 4;78,80,49,48;,
 4;80,72,44,49;,
 4;80,78,70,72;,
 4;79,81,73,71;,
 4;82,83,53,52;,
 4;74,82,52,46;,
 4;83,82,74,75;,
 4;84,76,47,54;,
 4;84,85,77,76;,
 4;83,86,55,53;,
 4;86,78,48,55;,
 4;86,83,75,78;,
 4;85,87,79,77;,
 4;88,89,81,79;,
 4;90,88,79,56;,
 4;91,90,56,51;,
 4;89,91,51,81;,
 4;89,88,90,91;,
 4;92,93,85,84;,
 4;94,92,84,54;,
 4;95,94,54,96;,
 4;93,92,94,95;,
 4;93,97,87,85;,
 4;98,95,96,57;,
 4;97,98,57,87;,
 4;97,93,95,98;,
 4;99,100,51,56;,
 4;101,102,25,24;,
 4;100,101,24,51;,
 4;100,99,102,101;,
 4;103,104,96,54;,
 4;105,103,54,32;,
 4;106,105,32,37;,
 4;104,103,105,106;,
 4;104,107,57,96;,
 4;108,106,37,36;,
 4;107,108,36,57;,
 4;107,104,106,108;,
 4;109,110,32,33;,
 4;111,109,33,38;,
 4;112,111,38,37;,
 4;112,110,109,111;,
 4;113,114,5,4;,
 4;115,113,4,45;,
 4;115,116,114,113;,
 4;117,118,50,14;,
 4;114,117,14,5;,
 4;116,118,117,114;,
 4;119,120,65,64;,
 4;120,119,116,115;,
 4;120,121,72,65;,
 4;122,115,45,44;,
 4;121,122,44,72;,
 4;121,120,115,122;,
 4;123,119,64,73;,
 4;119,123,118,116;,
 4;124,123,73,81;,
 4;125,124,81,51;,
 4;118,125,51,50;,
 4;123,124,125,118;,
 4;23,22,27,26;,
 4;39,33,31,35;;
 
 MeshMaterialList {
  17;
  114;
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
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  15,
  9,
  15,
  15,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9,
  9;;
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
  6;
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  114;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  126;
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
