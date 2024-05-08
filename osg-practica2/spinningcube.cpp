#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osg/Light>
#include <osg/LightSource>

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
    faces->push_back(0); faces->push_back(1); faces->push_back(2); faces->push_back(3);
    faces->push_back(7); faces->push_back(6); faces->push_back(5); faces->push_back(4);
    faces->push_back(0); faces->push_back(4); faces->push_back(5); faces->push_back(1);
    faces->push_back(3); faces->push_back(2); faces->push_back(6); faces->push_back(7);
    faces->push_back(1); faces->push_back(5); faces->push_back(6); faces->push_back(2);
    faces->push_back(0); faces->push_back(3); faces->push_back(7); faces->push_back(4);

    geom->addPrimitiveSet(faces.get());

    // Textura
    osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array(24);
    (*texcoords)[0].set(0.0f, 1.0f); (*texcoords)[1].set(1.0f, 1.0f); (*texcoords)[2].set(1.0f, 0.0f); (*texcoords)[3].set(0.0f, 0.0f);
    (*texcoords)[4].set(1.0f, 0.0f); (*texcoords)[5].set(0.0f, 0.0f); (*texcoords)[6].set(0.0f, 1.0f); (*texcoords)[7].set(1.0f, 1.0f);

    geom->setTexCoordArray(0, texcoords.get());

    return geom.release();
}

osg::MatrixTransform* createTransformedCube(float size, const osg::Vec3& axis, const osg::Vec3& position, osg::Image* image) {

    // Geometría del cubo
    osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;
    transform->setMatrix(osg::Matrix::rotate(0.0, axis) * osg::Matrix::translate(position));

    // Posición y rotación del cubo
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> geom = createCube(size);
    geode->addDrawable(geom.get());

    // Textura
    if (image) {
        osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
        texture->setImage(image);

        osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
        stateSet->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
        stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    }

    transform->addChild(geode.get());
    return transform.release();
}

osg::MatrixTransform* createLightCube(float size) {
    osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> geom = createCube(size);
    geode->addDrawable(geom.get());

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
    (*colors)[0] = osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    geom->setColorArray(colors.get(), osg::Array::BIND_OVERALL);

    transform->addChild(geode.get());

    return transform.release();
}

int main() {
    // Nodo raíz
    osg::ref_ptr<osg::Group> root = new osg::Group;

    // Cargar textura
    osg::Image* textureImage = osgDB::readImageFile("texture.jpg");

    // Primer cubo
    osg::ref_ptr<osg::MatrixTransform> cube1 = createTransformedCube(1.0f, osg::Vec3(0.0, 1.0, 1.0), osg::Vec3(-2.0, 0.0, 0.0), textureImage);
    root->addChild(cube1.get());

    // Segundo cubo
    osg::ref_ptr<osg::MatrixTransform> cube2 = createTransformedCube(1.0f, osg::Vec3(1.0, 0.0, 1.0), osg::Vec3(2.0, 0.0, 0.0), textureImage);
    root->addChild(cube2.get());

    // Cubo de luz
    osg::ref_ptr<osg::Light> light = new osg::Light;
    light->setLightNum(0);
    light->setPosition(osg::Vec4(0.0f, 0.0f, 5.0f, 1.0f));
    light->setAmbient(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
    light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setSpecular(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
    lightSource->setLight(light.get());

    osg::ref_ptr<osg::MatrixTransform> lightTransform = createLightCube(0.2f);
    osg::Matrix cubeLightMatrix = osg::Matrix::translate(osg::Vec3(0.0f, 0.0f, 5.0f));
    lightTransform->setMatrix(cubeLightMatrix);

    root->addChild(lightSource.get());
    root->addChild(lightTransform.get());

    // Visor OSG
    osgViewer::Viewer viewer;
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.setSceneData(root.get());

    while (!viewer.done()) {
        double time = viewer.elapsedTime();

        cube1->setMatrix(osg::Matrix::rotate(time * 0.5, osg::Vec3(0.0, 1.0, 1.0)) * osg::Matrix::translate(-2.0, 0.0, 0.0));
        cube2->setMatrix(osg::Matrix::rotate(time * 0.5, osg::Vec3(1.0, 0.0, 1.0)) * osg::Matrix::translate(2.0, 0.0, 0.0));

        viewer.frame();
    }

    return 0;
}
