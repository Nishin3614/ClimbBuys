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
 56;
 -11.90131;27.26198;-11.85813;,
 11.90131;27.26198;-11.85813;,
 11.90131;25.72282;-13.50000;,
 -11.90131;25.72282;-13.50000;,
 -13.50000;25.72282;-11.31487;,
 -11.90131;1.80116;-13.50000;,
 -13.50000;1.80116;-11.31487;,
 11.90131;1.80116;-13.50000;,
 13.50000;25.72282;-11.31487;,
 13.50000;1.80116;-11.31487;,
 11.90131;0.26198;-11.74043;,
 -11.90131;0.26198;-11.74043;,
 11.90131;27.26198;-11.85813;,
 11.90131;27.26198;11.85813;,
 13.50000;25.72282;11.69845;,
 13.50000;25.72282;-11.31487;,
 13.50000;1.80116;11.69845;,
 13.50000;1.80116;-11.31487;,
 11.90131;27.26198;11.85813;,
 -11.90131;27.26198;11.85813;,
 -11.90131;25.72282;13.50000;,
 11.90131;25.72282;13.50000;,
 13.50000;25.72282;11.69845;,
 11.90131;1.80116;13.50000;,
 13.50000;1.80116;11.69845;,
 -11.90131;1.80116;13.50000;,
 -13.50000;25.72282;11.69845;,
 -13.50000;1.80116;11.69845;,
 -11.90131;0.26198;11.74043;,
 11.90131;0.26198;11.74043;,
 -11.90131;27.26198;11.85813;,
 -11.90131;27.26198;-11.85813;,
 -13.50000;25.72282;-11.31487;,
 -13.50000;25.72282;11.69845;,
 -13.50000;1.80116;-11.31487;,
 -13.50000;1.80116;11.69845;,
 -11.90131;27.26198;11.85813;,
 11.90131;27.26198;11.85813;,
 11.90131;27.26198;-11.85813;,
 -11.90131;27.26198;-11.85813;,
 -13.50000;1.80116;-11.31487;,
 -11.90131;0.26198;-11.74043;,
 -11.90131;0.26198;11.74043;,
 -13.50000;1.80116;11.69845;,
 11.90131;0.26198;-11.74043;,
 11.90131;0.26198;11.74043;,
 13.50000;1.80116;-11.31487;,
 13.50000;1.80116;11.69845;,
 11.90131;27.26198;11.85813;,
 11.90131;27.26198;-11.85813;,
 -11.90131;27.26198;11.85813;,
 -11.90131;27.26198;-11.85813;,
 13.50000;1.80116;11.69845;,
 13.50000;1.80116;-11.31487;,
 -13.50000;1.80116;-11.31487;,
 -13.50000;1.80116;11.69845;;
 
 26;
 4;0,1,2,3;,
 4;4,3,5,6;,
 4;3,2,7,5;,
 4;2,8,9,7;,
 4;5,7,10,11;,
 4;12,13,14,15;,
 4;15,14,16,17;,
 4;18,19,20,21;,
 4;22,21,23,24;,
 4;21,20,25,23;,
 4;20,26,27,25;,
 4;23,25,28,29;,
 4;30,31,32,33;,
 4;33,32,34,35;,
 4;36,37,38,39;,
 4;40,41,42,43;,
 4;41,44,45,42;,
 4;44,46,47,45;,
 3;48,21,22;,
 3;49,8,2;,
 3;50,26,20;,
 3;51,3,4;,
 3;52,23,29;,
 3;53,10,7;,
 3;54,5,11;,
 3;55,28,25;;
 
 MeshMaterialList {
  2;
  26;
  1,
  1,
  0,
  1,
  1,
  1,
  0,
  1,
  1,
  0,
  1,
  1,
  1,
  0,
  0,
  1,
  0,
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
   0.800000;0.800000;0.800000;1.000000;;
   12.000000;
   0.000000;0.000000;0.000000;;
   0.110000;0.110000;0.110000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  26;
  -0.261336;0.483155;-0.835623;,
  0.439191;0.372150;-0.817690;,
  -0.437181;-0.388652;-0.811062;,
  0.437181;-0.388652;-0.811062;,
  0.394189;0.854244;-0.338941;,
  0.370760;0.860874;0.348473;,
  0.886572;0.349043;-0.303576;,
  0.864853;0.367891;0.341594;,
  0.884184;-0.357033;-0.301242;,
  0.862455;-0.376188;0.338606;,
  0.230144;0.492911;0.839090;,
  -0.393731;0.378952;0.837479;,
  0.391609;-0.395492;0.830800;,
  -0.391609;-0.395492;0.830800;,
  -0.370760;0.860874;0.348473;,
  -0.394189;0.854244;-0.338941;,
  -0.864853;0.367891;0.341594;,
  -0.886572;0.349043;-0.303576;,
  -0.862455;-0.376188;0.338606;,
  -0.884184;-0.357033;-0.301242;,
  -0.364076;-0.868958;0.335201;,
  0.364076;-0.868958;0.335201;,
  -0.387391;-0.862221;-0.326348;,
  0.387391;-0.862221;-0.326348;,
  -0.807062;0.000000;-0.590467;,
  0.747962;0.000000;0.663742;;
  26;
  4;15,4,1,0;,
  4;17,24,2,19;,
  4;0,1,3,2;,
  4;1,6,8,3;,
  4;2,3,23,22;,
  4;4,5,7,6;,
  4;6,7,9,8;,
  4;5,14,11,10;,
  4;7,25,12,9;,
  4;10,11,13,12;,
  4;11,16,18,13;,
  4;12,13,20,21;,
  4;14,15,17,16;,
  4;16,17,19,18;,
  4;14,5,4,15;,
  4;19,22,20,18;,
  4;22,23,21,20;,
  4;23,8,9,21;,
  3;5,10,7;,
  3;4,6,1;,
  3;14,16,11;,
  3;15,0,17;,
  3;9,12,21;,
  3;8,23,3;,
  3;19,2,22;,
  3;18,20,13;;
 }
 MeshTextureCoords {
  56;
  0.333330;0.000000;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.666670;1.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  0.333330;0.333330;,
  0.666670;0.666670;,
  0.333330;0.666670;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  0.333330;0.333330;,
  0.000000;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.666670;1.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;0.000000;,
  0.666670;0.333330;,
  0.333330;0.333330;,
  0.666670;0.666670;,
  0.333330;0.666670;,
  0.333330;0.333330;,
  0.666670;0.333330;,
  0.666670;0.666670;,
  0.333330;0.666670;,
  0.000000;0.333330;,
  0.333330;0.333330;,
  0.333330;0.666670;,
  0.000000;0.666670;,
  0.666670;0.333330;,
  0.666670;0.666670;,
  1.000000;0.333330;,
  1.000000;0.666670;,
  0.166670;0.000000;,
  0.833330;0.000000;,
  0.833330;0.000000;,
  0.166670;0.000000;,
  0.000000;0.833330;,
  1.000000;0.833330;,
  0.000000;0.833330;,
  1.000000;0.833330;;
 }
}
