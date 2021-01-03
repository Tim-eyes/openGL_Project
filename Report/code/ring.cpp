 Torus myTorus = Torus(1.5f, 0.15f, 48);

for (int i = 0; i < myTorus.getNumVertices(); i++) {
    pvalues1.push_back(vert1[i].x);
    vert1[i].y = 0;//torus thickness
    pvalues1.push_back(vert1[i].y);
    pvalues1.push_back(vert1[i].z);
    tvalues1.push_back(tex1[i].s);
    tvalues1.push_back(tex1[i].t);
    nvalues1.push_back(norm1[i].x);
    nvalues1.push_back(norm1[i].y);
    nvalues1.push_back(norm1[i].z);
}