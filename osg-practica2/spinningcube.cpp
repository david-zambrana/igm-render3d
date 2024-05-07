#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/Geode>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>

osg::Geometry* createCube(float size) {

    // Vertices del cubo
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(8);
    (*vertices)[0].set(-size, -size, -size);
    (*vertices)[1].set(size, -size, -size);
    (*vertices)[2].set(size, size, -size);
    (*vertices)[3].set(-size, size, -size);
    (*vertices)[4].set(-size, -size, size);
    (*vertices)[5].set(size, -size, size);
    (*vertices)[6].set(size, size, size);
    (*vertices)[7].set(-size, size, size);

    // Caras del cubo
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->setVertexArray(vertices.get());

    osg::ref_ptr<osg::DrawElementsUInt> faces = new osg::DrawElementsUInt(GL_QUADS, 24);
    faces->push_back(0); faces->push_back(1); faces->push_back(2); faces->push_back(3); // Front
    faces->push_back(7); faces->push_back(6); faces->push_back(5); faces->push_back(4); // Back
    faces->push_back(0); faces->push_back(4); faces->push_back(5); faces->push_back(1); // Bottom
    faces->push_back(3); faces->push_back(2); faces->push_back(6); faces->push_back(7); // Top
    faces->push_back(1); faces->push_back(5); faces->push_back(6); faces->push_back(2); // Right
    faces->push_back(0); faces->push_back(3); faces->push_back(7); faces->push_back(4); // Left

    geom->addPrimitiveSet(faces.get());
    return geom.release();
}

int main() {
    // Geometría del cubo
    osg::ref_ptr<osg::Geometry> cubeGeometry = createCube(1.0f);
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(cubeGeometry.get());

    // Posición y rotación del cubo
    osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;
    transform->addChild(geode.get());

    // Nodo raíz
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(transform.get());

    // Visor OSG
    osgViewer::Viewer viewer;
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);

    // Renderizar escena
    viewer.setSceneData(root.get());

    // Rotar el cubo
    while (!viewer.done()) {
        double time = viewer.elapsedTime();
        osg::Matrix rotationMatrix;
        rotationMatrix.makeRotate(time * 0.5, osg::Vec3(0.0, 1.0, 1.0));
        transform->setMatrix(rotationMatrix);
        viewer.frame();
    }

    return 0;
}
