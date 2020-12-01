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
 79;
 1.53141;4.61257;-0.32276;,
 0.00000;4.61257;-0.32276;,
 1.10674;17.86064;-0.32276;,
 0.00000;17.86064;-0.32276;,
 -1.53141;4.61257;-0.32276;,
 -1.10674;17.86064;-0.32276;,
 -1.10674;21.16553;-0.32276;,
 -1.53141;22.26715;-0.24207;,
 -1.14856;22.26715;-0.32276;,
 -1.53141;22.26715;0.24207;,
 -1.10674;21.16553;0.32276;,
 -1.14856;22.26715;0.32276;,
 0.00000;17.86064;0.32276;,
 0.00000;4.61257;0.32276;,
 1.10674;17.86064;0.32276;,
 1.53141;4.61257;0.32276;,
 -1.10674;17.86064;0.32276;,
 -1.53141;4.61257;0.32276;,
 1.53141;4.61257;0.00000;,
 0.00000;4.61257;0.00000;,
 -1.53141;4.61257;0.00000;,
 1.53141;4.61257;-0.32276;,
 0.00000;4.61257;-0.32276;,
 -1.53141;4.61257;-0.32276;,
 -1.53141;4.61257;0.00000;,
 -1.53141;4.61257;0.32276;,
 -1.10674;17.86064;0.00000;,
 -1.10674;17.86064;0.32276;,
 1.53141;4.61257;0.32276;,
 1.53141;4.61257;0.00000;,
 1.10674;17.86064;0.32276;,
 1.10674;17.86064;0.00000;,
 1.10674;21.16553;0.32276;,
 1.53141;22.26715;0.24207;,
 1.14856;22.26715;0.32276;,
 1.53141;22.26715;-0.24207;,
 1.10674;21.16553;-0.32276;,
 1.14856;22.26715;-0.32276;,
 -1.53141;22.26715;0.00000;,
 -1.53141;22.26715;-0.24207;,
 0.00000;22.26715;-0.32276;,
 0.00000;27.39233;0.00000;,
 -1.53141;22.26715;0.00000;,
 -1.10674;21.16553;0.32276;,
 0.00000;22.26715;0.32276;,
 -1.53141;22.26715;0.24207;,
 1.53141;22.26715;0.00000;,
 1.53141;22.26715;-0.24207;,
 1.53141;22.26715;0.00000;,
 1.10674;21.16553;0.32276;,
 1.53141;22.26715;0.24207;,
 5.92120;2.73172;-0.56045;,
 -5.92120;2.73172;-0.56045;,
 3.40158;4.73944;-0.56045;,
 -3.40158;4.73944;-0.56045;,
 3.40158;4.73944;0.56045;,
 -3.40158;4.73944;0.56045;,
 5.92120;2.73172;0.56045;,
 -5.92120;2.73172;0.56045;,
 5.92120;2.73172;-0.56045;,
 -5.92120;2.73172;-0.56045;,
 -5.92120;2.73172;0.56045;,
 -3.40158;4.73944;0.56045;,
 5.92120;2.73172;0.56045;,
 3.40158;4.73944;0.56045;,
 0.68834;-2.80607;-0.26777;,
 -0.68834;-2.80607;-0.26777;,
 0.68834;3.32861;-0.26777;,
 -0.68834;3.32861;-0.26777;,
 0.68834;3.32861;0.26777;,
 -0.68834;3.32861;0.26777;,
 0.68834;-2.80607;0.26777;,
 -0.68834;-2.80607;0.26777;,
 0.68834;-2.80607;-0.26777;,
 -0.68834;-2.80607;-0.26777;,
 -0.68834;-2.80607;0.26777;,
 -0.68834;3.32861;0.26777;,
 0.68834;-2.80607;0.26777;,
 0.68834;3.32861;0.26777;;
 
 88;
 3;0,1,2;,
 3;2,1,3;,
 3;4,5,1;,
 3;1,5,3;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;14,13,15;,
 3;12,16,13;,
 3;13,16,17;,
 3;15,13,18;,
 3;18,13,19;,
 3;13,17,19;,
 3;19,17,20;,
 3;18,19,21;,
 3;21,19,22;,
 3;19,20,22;,
 3;22,20,23;,
 3;24,25,26;,
 3;26,25,27;,
 3;4,24,5;,
 3;5,24,26;,
 3;28,29,30;,
 3;30,29,31;,
 3;29,0,31;,
 3;31,0,2;,
 3;32,33,34;,
 3;35,36,37;,
 3;26,38,5;,
 3;38,39,5;,
 3;5,39,6;,
 3;40,8,41;,
 3;42,41,7;,
 3;8,7,41;,
 3;3,5,40;,
 3;5,6,40;,
 3;6,8,40;,
 3;26,27,38;,
 3;38,27,9;,
 3;27,43,9;,
 3;44,11,12;,
 3;11,10,12;,
 3;10,16,12;,
 3;44,41,11;,
 3;42,45,41;,
 3;41,45,11;,
 3;3,40,2;,
 3;2,40,36;,
 3;40,37,36;,
 3;40,41,37;,
 3;46,47,41;,
 3;47,37,41;,
 3;31,2,48;,
 3;48,2,35;,
 3;2,36,35;,
 3;44,34,41;,
 3;46,41,33;,
 3;41,34,33;,
 3;44,12,34;,
 3;34,12,49;,
 3;49,12,14;,
 3;31,48,30;,
 3;48,50,30;,
 3;30,50,32;,
 3;51,52,53;,
 3;53,52,54;,
 3;53,54,55;,
 3;55,54,56;,
 3;55,56,57;,
 3;57,56,58;,
 3;57,58,59;,
 3;59,58,60;,
 3;52,61,54;,
 3;54,61,62;,
 3;63,51,64;,
 3;64,51,53;,
 3;65,66,67;,
 3;67,66,68;,
 3;67,68,69;,
 3;69,68,70;,
 3;69,70,71;,
 3;71,70,72;,
 3;71,72,73;,
 3;73,72,74;,
 3;66,75,68;,
 3;68,75,76;,
 3;77,65,78;,
 3;78,65,67;;
 
 MeshMaterialList {
  1;
  88;
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
  0;;
  Material {
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  50;
  0.000000;0.000000;-1.000000;,
  0.037440;0.000000;-0.999299;,
  -0.037440;0.000000;-0.999299;,
  0.000000;0.015718;-0.999876;,
  0.999488;0.032007;0.000000;,
  0.000000;0.085971;-0.996298;,
  -0.999488;0.032007;0.000000;,
  0.000000;0.031441;0.999506;,
  0.046822;0.000000;0.998903;,
  0.000000;0.000000;1.000000;,
  -0.046822;0.000000;0.998903;,
  0.000000;-1.000000;-0.000000;,
  -0.999816;-0.019186;0.000000;,
  0.999816;-0.019186;-0.000000;,
  -0.103492;0.040825;-0.993792;,
  -0.082768;0.032650;0.996034;,
  -0.098711;-0.001967;-0.995114;,
  -0.199602;0.033177;-0.979315;,
  -0.199602;0.033177;0.979315;,
  -0.098711;-0.001967;0.995114;,
  0.103492;0.040825;-0.993792;,
  0.082768;0.032650;0.996034;,
  0.098711;-0.001967;-0.995114;,
  0.199602;0.033177;-0.979315;,
  0.199602;0.033177;0.979315;,
  0.098711;-0.001967;0.995114;,
  0.000000;0.000000;-1.000000;,
  0.330095;0.943948;0.000000;,
  -0.330095;0.943948;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.999487;0.032039;0.000000;,
  -0.999487;0.032039;0.000000;,
  -0.998575;-0.053370;0.000000;,
  -0.999488;-0.032011;0.000000;,
  0.999487;0.032039;0.000000;,
  0.999487;0.032039;0.000000;,
  0.999488;-0.032011;-0.000000;,
  0.998575;-0.053370;-0.000000;,
  -0.995286;0.096987;0.000000;,
  -0.958142;0.286294;0.000000;,
  0.000000;0.085971;0.996298;,
  0.995286;0.096987;0.000000;,
  0.958142;0.286294;0.000000;,
  -0.623185;0.782075;0.000000;,
  -0.623185;0.782075;0.000000;,
  0.623185;0.782075;0.000000;,
  0.623185;0.782075;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  88;
  3;0,0,1;,
  3;1,0,0;,
  3;0,2,0;,
  3;0,2,0;,
  3;16,17,14;,
  3;18,19,15;,
  3;9,9,8;,
  3;8,9,9;,
  3;9,10,9;,
  3;9,10,9;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;11,11,11;,
  3;31,32,12;,
  3;12,32,33;,
  3;32,31,34;,
  3;34,31,12;,
  3;35,36,37;,
  3;37,36,13;,
  3;36,35,13;,
  3;13,35,38;,
  3;25,24,21;,
  3;23,22,20;,
  3;12,6,34;,
  3;6,39,34;,
  3;2,17,16;,
  3;3,14,5;,
  3;6,40,39;,
  3;14,17,5;,
  3;0,2,3;,
  3;2,16,3;,
  3;16,14,3;,
  3;12,33,6;,
  3;6,33,39;,
  3;10,19,18;,
  3;7,15,9;,
  3;15,19,9;,
  3;19,10,9;,
  3;7,41,15;,
  3;6,39,40;,
  3;41,18,15;,
  3;0,3,1;,
  3;1,3,22;,
  3;3,20,22;,
  3;3,5,20;,
  3;4,42,43;,
  3;23,20,5;,
  3;13,38,4;,
  3;4,38,42;,
  3;1,22,23;,
  3;7,21,41;,
  3;4,43,42;,
  3;41,21,24;,
  3;7,9,21;,
  3;21,9,25;,
  3;25,9,8;,
  3;13,4,37;,
  3;4,42,37;,
  3;8,24,25;,
  3;0,0,0;,
  3;0,0,26;,
  3;27,28,27;,
  3;27,28,28;,
  3;30,9,9;,
  3;9,9,9;,
  3;11,11,11;,
  3;11,11,11;,
  3;44,45,28;,
  3;28,45,28;,
  3;46,47,27;,
  3;27,47,27;,
  3;26,0,0;,
  3;0,0,26;,
  3;29,29,29;,
  3;29,29,29;,
  3;30,9,9;,
  3;9,9,30;,
  3;11,11,11;,
  3;11,11,11;,
  3;48,48,48;,
  3;48,48,48;,
  3;49,49,49;,
  3;49,49,49;;
 }
 MeshTextureCoords {
  79;
  0.375000;0.000000;,
  0.500000;0.000000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.625000;0.000000;,
  0.625000;0.125000;,
  0.625000;0.187500;,
  0.625000;0.312500;,
  0.562500;0.250000;,
  0.812500;0.250000;,
  0.625000;0.562500;,
  0.562500;0.500000;,
  0.500000;0.625000;,
  0.500000;0.750000;,
  0.375000;0.625000;,
  0.375000;0.750000;,
  0.625000;0.625000;,
  0.625000;0.750000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.875000;0.000000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.125000;0.000000;,
  0.250000;0.000000;,
  0.125000;0.125000;,
  0.250000;0.125000;,
  0.125000;0.187500;,
  0.375000;0.437500;,
  0.437500;0.500000;,
  0.312500;0.250000;,
  0.375000;0.187500;,
  0.437500;0.250000;,
  0.750000;0.250000;,
  0.687500;0.250000;,
  0.500000;0.250000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.875000;0.187500;,
  0.500000;0.500000;,
  0.625000;0.437500;,
  0.375000;0.375000;,
  0.375000;0.312500;,
  0.250000;0.250000;,
  0.375000;0.562500;,
  0.187500;0.250000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.375000;0.500000;,
  0.625000;0.500000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.125000;0.000000;,
  0.125000;0.250000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.375000;0.250000;,
  0.625000;0.250000;,
  0.375000;0.500000;,
  0.625000;0.500000;,
  0.375000;0.750000;,
  0.625000;0.750000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;0.250000;,
  0.125000;0.000000;,
  0.125000;0.250000;;
 }
}
