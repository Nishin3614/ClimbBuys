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
 102;
 3.16244;0.60724;-1.40095;,
 1.99761;0.60724;-1.40095;,
 1.99761;0.60724;1.45289;,
 3.16244;0.60724;1.45289;,
 3.16244;-1.35276;1.45289;,
 3.16244;-1.35276;-1.40095;,
 3.16244;0.60724;-2.28463;,
 3.16244;-1.35276;-2.28463;,
 1.99761;0.60724;-2.28463;,
 3.16244;-1.35276;2.33657;,
 3.16244;0.60724;2.33657;,
 1.99761;0.60724;2.33657;,
 0.87295;0.60724;-3.24864;,
 0.87295;0.60724;-2.28463;,
 1.99761;0.60724;-3.24864;,
 1.99761;0.60724;3.30058;,
 0.87295;0.60724;2.33657;,
 0.87295;0.60724;3.30058;,
 3.16244;0.60724;-3.24864;,
 3.16244;-1.33623;-3.24864;,
 3.16244;-1.35276;3.30058;,
 3.16244;0.60724;3.30058;,
 -0.05088;0.60724;-4.35324;,
 -0.05088;0.60724;-3.24864;,
 0.87295;0.60724;-4.35324;,
 0.87295;-1.35276;-4.35324;,
 -0.05088;-1.35276;-4.35324;,
 0.87295;0.60724;4.40518;,
 -0.05088;0.60724;3.30058;,
 -0.05088;0.60724;4.40518;,
 -0.05088;-1.35276;4.40518;,
 0.87295;-1.35276;4.40518;,
 1.99761;0.60724;-4.35324;,
 1.99761;-1.35276;-4.35324;,
 1.99761;0.60724;4.40518;,
 1.99761;-1.35276;4.40518;,
 1.99761;-1.35276;-3.24864;,
 1.99761;-1.35276;3.30058;,
 1.99761;1.53111;1.45289;,
 1.99761;1.53111;-1.40095;,
 0.87295;1.53111;-1.40095;,
 0.87295;1.53111;1.45289;,
 -0.05088;1.53111;-2.28463;,
 -0.05088;1.53111;-1.40095;,
 0.87295;1.53111;-2.28463;,
 0.87295;1.53111;2.33657;,
 -0.05088;1.53111;1.45289;,
 -0.05088;1.53111;2.33657;,
 1.99761;1.53111;-2.28463;,
 1.99761;1.53111;2.33657;,
 0.87295;1.53111;-3.24864;,
 -0.05088;1.53111;-3.24864;,
 -0.05088;1.53111;3.30058;,
 0.87295;1.53111;3.30058;,
 0.87295;2.35643;1.45289;,
 0.87295;2.35643;-1.40095;,
 -0.05088;2.35643;1.45289;,
 -0.05088;2.35643;-1.40095;,
 -0.05088;2.35643;2.33657;,
 -0.05088;1.53111;2.33657;,
 -0.05088;1.53111;1.45289;,
 -0.05088;2.35643;1.45289;,
 -2.51357;-3.75192;1.45289;,
 -2.51357;-1.35276;1.45289;,
 -2.51357;-1.35276;-1.40095;,
 -2.51357;-3.75192;-1.40095;,
 3.16244;-3.75192;-1.40095;,
 3.16244;-1.35276;-1.40095;,
 3.16244;-1.35276;1.45289;,
 3.16244;-3.75192;1.45289;,
 -2.51357;-1.35276;-2.28463;,
 -2.51357;-3.75192;-2.28463;,
 -2.51357;-3.75192;2.33657;,
 -2.51357;-1.35276;2.33657;,
 3.16244;-3.75192;-2.28463;,
 3.16244;-1.35276;-2.28463;,
 3.16244;-1.35276;2.33657;,
 3.16244;-3.75192;2.33657;,
 -2.51357;-1.35276;-3.24864;,
 -2.51357;-3.75192;-3.24864;,
 -2.51357;-3.75192;3.30058;,
 -2.51357;-1.35276;3.30058;,
 1.99761;-3.75192;-3.24864;,
 1.99761;-1.35276;-3.24864;,
 3.16244;-1.35447;-3.24864;,
 3.16244;-3.75192;-3.24864;,
 3.16244;-3.75192;3.30058;,
 3.16244;-1.35276;3.30058;,
 1.99761;-1.35276;3.30058;,
 1.99761;-3.75192;3.30058;,
 -2.51357;-3.75192;-4.35324;,
 -2.51357;-1.35276;-4.35324;,
 0.87295;-1.35276;-4.35324;,
 0.87295;-3.75192;-4.35324;,
 0.87295;-3.75192;4.40518;,
 0.87295;-1.35276;4.40518;,
 -2.51357;-1.35276;4.40518;,
 -2.51357;-3.75192;4.40518;,
 1.99761;-1.35276;-4.35324;,
 1.99761;-3.75192;-4.35324;,
 1.99761;-3.75192;4.40518;,
 1.99761;-1.35276;4.40518;;
 
 69;
 4;0,1,2,3;,
 4;3,4,5,0;,
 4;6,0,5,7;,
 4;8,1,0,6;,
 4;9,4,3,10;,
 4;10,3,2,11;,
 4;12,13,8,14;,
 4;15,11,16,17;,
 4;18,6,7,19;,
 4;14,8,6,18;,
 4;20,9,10,21;,
 4;21,10,11,15;,
 4;22,23,12,24;,
 4;25,26,22,24;,
 4;27,17,28,29;,
 4;27,29,30,31;,
 4;24,12,14,32;,
 4;33,25,24,32;,
 4;34,15,17,27;,
 4;34,27,31,35;,
 4;14,18,19,36;,
 4;36,33,32,14;,
 4;37,20,21,15;,
 4;15,34,35,37;,
 4;38,2,1,39;,
 4;39,40,41,38;,
 4;42,43,40,44;,
 4;45,41,46,47;,
 4;48,8,13,44;,
 4;39,1,8,48;,
 4;44,40,39,48;,
 4;45,16,11,49;,
 4;49,11,2,38;,
 4;49,38,41,45;,
 4;50,12,23,51;,
 4;44,13,12,50;,
 4;51,42,44,50;,
 4;52,28,17,53;,
 4;53,17,16,45;,
 4;53,45,47,52;,
 4;54,41,40,55;,
 4;56,46,41,54;,
 4;55,40,43,57;,
 4;55,57,56,54;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;65,64,70,71;,
 4;72,73,63,62;,
 4;74,75,67,66;,
 4;69,68,76,77;,
 4;71,70,78,79;,
 4;80,81,73,72;,
 4;82,83,84,85;,
 4;85,84,75,74;,
 4;86,87,88,89;,
 4;77,76,87,86;,
 4;90,91,92,93;,
 4;79,78,91,90;,
 4;94,95,96,97;,
 4;97,96,81,80;,
 4;93,92,98,99;,
 4;99,98,83,82;,
 4;100,101,95,94;,
 4;89,88,101,100;,
 4;35,33,91,96;,
 4;29,22,26,30;,
 4;52,51,23,28;,
 4;43,46,56,57;;
 
 MeshMaterialList {
  17;
  69;
  15,
  15,
  15,
  15,
  15,
  15,
  16,
  15,
  15,
  15,
  15,
  15,
  16,
  16,
  15,
  15,
  16,
  16,
  15,
  15,
  16,
  16,
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
  16,
  16,
  16,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  15,
  16,
  15,
  16,
  16,
  15,
  15,
  16,
  16,
  16,
  15,
  15,
  15,
  16,
  16,
  15,
  15,
  16,
  16,
  15,
  15,
  8,
  16,
  16,
  16;;
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
  5;
  1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;;
  69;
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;4,4,4,4;;
 }
 MeshTextureCoords {
  102;
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.000000;,
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
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
