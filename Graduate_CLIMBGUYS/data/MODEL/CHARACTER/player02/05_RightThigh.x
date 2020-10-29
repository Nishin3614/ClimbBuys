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
 -3.10012;1.17886;-1.56822;,
 -3.10012;1.17886;1.28562;,
 -1.93529;1.17886;1.28562;,
 -1.93529;1.17886;-1.56822;,
 -3.10012;-0.78114;-1.56822;,
 -3.10012;-0.78114;1.28562;,
 -3.10012;1.17886;-2.45190;,
 -3.10012;-0.78114;-2.45190;,
 -1.93529;1.17886;-2.45190;,
 -3.10012;-0.78114;2.16930;,
 -3.10012;1.17886;2.16930;,
 -1.93529;1.17886;2.16930;,
 -0.81063;1.17886;-3.41591;,
 -1.93529;1.17886;-3.41591;,
 -0.81063;1.17886;-2.45190;,
 -1.93529;1.17886;3.13331;,
 -0.81063;1.17886;3.13331;,
 -0.81063;1.17886;2.16930;,
 -3.10012;1.17886;-3.41591;,
 -3.10012;-0.76461;-3.41591;,
 -3.10012;-0.78114;3.13331;,
 -3.10012;1.17886;3.13331;,
 0.11320;1.17886;-4.52051;,
 -0.81063;1.17886;-4.52051;,
 0.11320;1.17886;-3.41591;,
 -0.81063;-0.78114;-4.52051;,
 0.11320;-0.78114;-4.52051;,
 -0.81063;1.17886;4.23791;,
 0.11320;1.17886;4.23791;,
 0.11320;1.17886;3.13331;,
 -0.81063;-0.78114;4.23791;,
 0.11320;-0.78114;4.23791;,
 -1.93529;1.17886;-4.52051;,
 -1.93529;-0.78114;-4.52051;,
 -1.93529;1.17886;4.23791;,
 -1.93529;-0.78114;4.23791;,
 -1.93529;-0.78114;-3.41591;,
 -1.93529;-0.78114;3.13331;,
 -1.93529;2.10273;1.28562;,
 -1.93529;2.10273;-1.56822;,
 -0.81063;2.10273;1.28562;,
 -0.81063;2.10273;-1.56822;,
 0.11320;2.10273;-2.45190;,
 -0.81063;2.10273;-2.45190;,
 0.11320;2.10273;-1.56822;,
 -0.81063;2.10273;2.16930;,
 0.11320;2.10273;2.16930;,
 0.11320;2.10273;1.28562;,
 -1.93529;2.10273;-2.45190;,
 -1.93529;2.10273;2.16930;,
 -0.81063;2.10273;-3.41591;,
 0.11320;2.10273;-3.41591;,
 0.11320;2.10273;3.13331;,
 -0.81063;2.10273;3.13331;,
 -0.81063;2.92805;1.28562;,
 -0.81063;2.92805;-1.56822;,
 0.11320;2.92805;1.28562;,
 0.11320;2.92805;-1.56822;,
 0.11320;2.92805;2.16930;,
 0.11320;2.92805;1.28562;,
 0.11320;2.10273;1.28562;,
 0.11320;2.10273;2.16930;,
 2.57589;-3.18030;1.28562;,
 2.57589;-3.18030;-1.56822;,
 2.57589;-0.78114;-1.56822;,
 2.57589;-0.78114;1.28562;,
 -3.10012;-3.18030;-1.56822;,
 -3.10012;-3.18030;1.28562;,
 -3.10012;-0.78114;1.28562;,
 -3.10012;-0.78114;-1.56822;,
 2.57589;-3.18030;-2.45190;,
 2.57589;-0.78114;-2.45190;,
 2.57589;-3.18030;2.16930;,
 2.57589;-0.78114;2.16930;,
 -3.10012;-3.18030;-2.45190;,
 -3.10012;-0.78114;-2.45190;,
 -3.10012;-3.18030;2.16930;,
 -3.10012;-0.78114;2.16930;,
 2.57589;-3.18030;-3.41591;,
 2.57589;-0.78114;-3.41591;,
 2.57589;-3.18030;3.13331;,
 2.57589;-0.78114;3.13331;,
 -1.93529;-3.18030;-3.41591;,
 -3.10012;-3.18030;-3.41591;,
 -3.10012;-0.78285;-3.41591;,
 -1.93529;-0.78114;-3.41591;,
 -3.10012;-3.18030;3.13331;,
 -1.93529;-3.18030;3.13331;,
 -1.93529;-0.78114;3.13331;,
 -3.10012;-0.78114;3.13331;,
 2.57589;-3.18030;-4.52051;,
 -0.81063;-3.18030;-4.52051;,
 -0.81063;-0.78114;-4.52051;,
 2.57589;-0.78114;-4.52051;,
 -0.81063;-3.18030;4.23791;,
 2.57589;-3.18030;4.23791;,
 2.57589;-0.78114;4.23791;,
 -0.81063;-0.78114;4.23791;,
 -1.93529;-3.18030;-4.52051;,
 -1.93529;-0.78114;-4.52051;,
 -1.93529;-3.18030;4.23791;,
 -1.93529;-0.78114;4.23791;;
 
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
 4;56,47,44,57;;
 
 MeshMaterialList {
  11;
  69;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  1,
  1,
  1,
  3,
  0,
  0,
  0;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   28.000000;
   0.120000;0.120000;0.120000;;
   0.110000;0.110000;0.110000;;
  }
  Material {
   0.000000;0.661600;0.800000;1.000000;;
   14.000000;
   0.100000;0.100000;0.100000;;
   0.000000;0.082700;0.100000;;
  }
  Material {
   0.800000;0.712000;0.486400;1.000000;;
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
   0.109600;0.050400;0.006400;1.000000;;
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
   0.800000;0.800000;0.800000;1.000000;;
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
   0.000000;0.119200;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.768800;0.000000;0.800000;1.000000;;
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