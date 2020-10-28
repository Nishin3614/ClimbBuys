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
 -3.16244;0.60724;-1.40095;,
 -3.16244;0.60724;1.45289;,
 -1.99761;0.60724;1.45289;,
 -1.99761;0.60724;-1.40095;,
 -3.16244;-1.35276;-1.40095;,
 -3.16244;-1.35276;1.45289;,
 -3.16244;0.60724;-2.28463;,
 -3.16244;-1.35276;-2.28463;,
 -1.99761;0.60724;-2.28463;,
 -3.16244;-1.35276;2.33657;,
 -3.16244;0.60724;2.33657;,
 -1.99761;0.60724;2.33657;,
 -0.87295;0.60724;-3.24864;,
 -1.99761;0.60724;-3.24864;,
 -0.87295;0.60724;-2.28463;,
 -1.99761;0.60724;3.30058;,
 -0.87295;0.60724;3.30058;,
 -0.87295;0.60724;2.33657;,
 -3.16244;0.60724;-3.24864;,
 -3.16244;-1.33623;-3.24864;,
 -3.16244;-1.35276;3.30058;,
 -3.16244;0.60724;3.30058;,
 0.05088;0.60724;-4.35324;,
 -0.87295;0.60724;-4.35324;,
 0.05088;0.60724;-3.24864;,
 -0.87295;-1.35276;-4.35324;,
 0.05088;-1.35276;-4.35324;,
 -0.87295;0.60724;4.40518;,
 0.05088;0.60724;4.40518;,
 0.05088;0.60724;3.30058;,
 -0.87295;-1.35276;4.40518;,
 0.05088;-1.35276;4.40518;,
 -1.99761;0.60724;-4.35324;,
 -1.99761;-1.35276;-4.35324;,
 -1.99761;0.60724;4.40518;,
 -1.99761;-1.35276;4.40518;,
 -1.99761;-1.35276;-3.24864;,
 -1.99761;-1.35276;3.30058;,
 -1.99761;1.53111;1.45289;,
 -1.99761;1.53111;-1.40095;,
 -0.87295;1.53111;1.45289;,
 -0.87295;1.53111;-1.40095;,
 0.05088;1.53111;-2.28463;,
 -0.87295;1.53111;-2.28463;,
 0.05088;1.53111;-1.40095;,
 -0.87295;1.53111;2.33657;,
 0.05088;1.53111;2.33657;,
 0.05088;1.53111;1.45289;,
 -1.99761;1.53111;-2.28463;,
 -1.99761;1.53111;2.33657;,
 -0.87295;1.53111;-3.24864;,
 0.05088;1.53111;-3.24864;,
 0.05088;1.53111;3.30058;,
 -0.87295;1.53111;3.30058;,
 -0.87295;2.35643;1.45289;,
 -0.87295;2.35643;-1.40095;,
 0.05088;2.35643;1.45289;,
 0.05088;2.35643;-1.40095;,
 0.05088;2.35643;2.33657;,
 0.05088;2.35643;1.45289;,
 0.05088;1.53111;1.45289;,
 0.05088;1.53111;2.33657;,
 2.51357;-3.75192;1.45289;,
 2.51357;-3.75192;-1.40095;,
 2.51357;-1.35276;-1.40095;,
 2.51357;-1.35276;1.45289;,
 -3.16244;-3.75192;-1.40095;,
 -3.16244;-3.75192;1.45289;,
 -3.16244;-1.35276;1.45289;,
 -3.16244;-1.35276;-1.40095;,
 2.51357;-3.75192;-2.28463;,
 2.51357;-1.35276;-2.28463;,
 2.51357;-3.75192;2.33657;,
 2.51357;-1.35276;2.33657;,
 -3.16244;-3.75192;-2.28463;,
 -3.16244;-1.35276;-2.28463;,
 -3.16244;-3.75192;2.33657;,
 -3.16244;-1.35276;2.33657;,
 2.51357;-3.75192;-3.24864;,
 2.51357;-1.35276;-3.24864;,
 2.51357;-3.75192;3.30058;,
 2.51357;-1.35276;3.30058;,
 -1.99761;-3.75192;-3.24864;,
 -3.16244;-3.75192;-3.24864;,
 -3.16244;-1.35447;-3.24864;,
 -1.99761;-1.35276;-3.24864;,
 -3.16244;-3.75192;3.30058;,
 -1.99761;-3.75192;3.30058;,
 -1.99761;-1.35276;3.30058;,
 -3.16244;-1.35276;3.30058;,
 2.51357;-3.75192;-4.35324;,
 -0.87295;-3.75192;-4.35324;,
 -0.87295;-1.35276;-4.35324;,
 2.51357;-1.35276;-4.35324;,
 -0.87295;-3.75192;4.40518;,
 2.51357;-3.75192;4.40518;,
 2.51357;-1.35276;4.40518;,
 -0.87295;-1.35276;4.40518;,
 -1.99761;-3.75192;-4.35324;,
 -1.99761;-1.35276;-4.35324;,
 -1.99761;-3.75192;4.40518;,
 -1.99761;-1.35276;4.40518;;
 
 69;
 4;0,1,2,3;,
 4;1,0,4,5;,
 4;6,7,4,0;,
 4;8,6,0,3;,
 4;9,10,1,5;,
 4;10,11,2,1;,
 4;12,13,8,14;,
 4;15,16,17,11;,
 4;18,19,7,6;,
 4;13,18,6,8;,
 4;20,21,10,9;,
 4;21,15,11,10;,
 4;22,23,12,24;,
 4;25,23,22,26;,
 4;27,28,29,16;,
 4;27,30,31,28;,
 4;23,32,13,12;,
 4;33,32,23,25;,
 4;34,27,16,15;,
 4;34,35,30,27;,
 4;13,36,19,18;,
 4;36,13,32,33;,
 4;37,15,21,20;,
 4;15,37,35,34;,
 4;38,39,3,2;,
 4;39,38,40,41;,
 4;42,43,41,44;,
 4;45,46,47,40;,
 4;48,43,14,8;,
 4;39,48,8,3;,
 4;43,48,39,41;,
 4;45,49,11,17;,
 4;49,38,2,11;,
 4;49,45,40,38;,
 4;50,51,24,12;,
 4;43,50,12,14;,
 4;51,50,43,42;,
 4;52,53,16,29;,
 4;53,45,17,16;,
 4;53,52,46,45;,
 4;54,55,41,40;,
 4;56,54,40,47;,
 4;55,57,44,41;,
 4;55,54,56,57;,
 4;58,59,60,61;,
 4;62,63,64,65;,
 4;66,67,68,69;,
 4;63,70,71,64;,
 4;72,62,65,73;,
 4;74,66,69,75;,
 4;67,76,77,68;,
 4;70,78,79,71;,
 4;80,72,73,81;,
 4;82,83,84,85;,
 4;83,74,75,84;,
 4;86,87,88,89;,
 4;76,86,89,77;,
 4;90,91,92,93;,
 4;78,90,93,79;,
 4;94,95,96,97;,
 4;95,80,81,96;,
 4;91,98,99,92;,
 4;98,82,85,99;,
 4;100,94,97,101;,
 4;87,100,101,88;,
 4;35,96,93,33;,
 4;28,31,26,22;,
 4;52,29,24,51;,
 4;44,57,56,47;;
 
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
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-0.000000;1.000000;,
  1.000000;0.000000;0.000000;;
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
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
  0.000000;0.000000;,
  1.000000;0.000000;,
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
  0.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;;
 }
}
