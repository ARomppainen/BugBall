#include "Shape.h"

#include "AssetFactory.h"

//
// ASSET CLASS
//

Shape::Shape()
{

}

Shape::~Shape()
{

}

/*
void Shape::render(Camera& c, Transform& t, Material& m)
{
	shaders->use();

    shaders->setUniform("camera", c.getViewProjectionMatrix());
    shaders->setUniform("model", t.getModelMatrix());

	bool useTexture = false;

	if (m.texture) {
        shaders->setUniform("material.texture", 0); // texture will be bound to GL_TEXTURE0
        useTexture = true;
	} else if (m.diffuseColor) {
        shaders->setUniform("material.diffuseColor", *(m.diffuseColor));
	} else {
        // set diffuse color to red if no color is set somehow
        vec4 col = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        shaders->setUniform("material.diffuseColor", col);
	}

    shaders->setUniform("useTexture", useTexture);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m.texture->getId());
    }

    // draw
    if (IwGLExtAvailable(IW_GL_OES_vertex_array_object)) {
        glBindVertexArrayOES(vao);
    }

    glDrawArrays(drawType, drawStart, drawCount);

    if (IwGLExtAvailable(IW_GL_OES_vertex_array_object)) {
        glBindVertexArrayOES(0);
    }

    if (useTexture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    shaders->stopUsing();
}
*/

//
// SPRITE CLASS
//

Sprite::Sprite()
{
    // TODO: check that this path is correct
    //Shader vs = Shader::shaderFromFile(GL_VERTEX_SHADER, "shaders/vs_simple.txt");
    //Shader fs = Shader::shaderFromFile(GL_FRAGMENT_SHADER, "shaders/fs_simple.txt");
    //program = new Program("shaders/vs_simple", "shaders/fs_simple");
    //program = AssetFactory::getInstance().getProgram("SIMPLE");

	drawType = GL_TRIANGLES;
	drawStart = 0;
	drawCount = 6;

    GLfloat vertexData[] = {
		 // vert XYZ,       UV
        -0.5f,-0.5f, 0.0f,  0.01f, 0.01f,
         0.5f,-0.5f, 0.0f,  0.99f, 0.01f,
        -0.5f, 0.5f, 0.0f,  0.01f, 0.99f,

         0.5f,-0.5f, 0.0f,  0.99f, 0.01f,
         0.5f, 0.5f, 0.0f,  0.99f, 0.99f,
        -0.5f, 0.5f, 0.0f,  0.01f, 0.99f,
	};

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
}

Sprite::~Sprite()
{

}

// http://stackoverflow.com/questions/8704801/glvertexattribpointer-clarification
void Sprite::render(
    Camera& c,
    Transform& t,
    Material& m,
    bool swapX,
    bool swapY)
{
    Program* program = AssetFactory::getInstance().getProgram("SIMPLE");

    if (AssetFactory::getInstance().getActiveProgram() != program->getId()) {
        program->use();
        AssetFactory::getInstance().setActiveProgram(program->getId());
    }

    // set uniforms
    program->setUniform("camera", c.getViewProjectionMatrix());
    program->setUniform("model", t.getModelMatrix());
    program->setUniform("swapX", swapX);
    program->setUniform("swapY", swapY);

    bool useTexture = false;

    if (m.texture) {
        program->setUniform("tex", 0); // texture will be bound to GL_TEXTURE0
        useTexture = true;
	} else if (m.color) {
        program->setUniform("color", *(m.color));
	} else {
        // set diffuse color to red if no color is set somehow
        vec4 col = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        program->setUniform("color", col);
	}

    program->setUniform("useTexture", useTexture);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);

        if (AssetFactory::getInstance().getActiveTexture() != m.texture->getId()) {
            glBindTexture(GL_TEXTURE_2D, m.texture->getId());
            AssetFactory::getInstance().setActiveTexture(m.texture->getId());
        }
    }

    // set attributes
    glEnableVertexAttribArray(program->attrib("vert"));
    glVertexAttribPointer(program->attrib("vert"),
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        0);

    glEnableVertexAttribArray(program->attrib("vertTexCoord"));
    glVertexAttribPointer(program->attrib("vertTexCoord"),
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        (const GLvoid*)(3 * sizeof(GLfloat)));

    glDrawArrays(drawType, drawStart, drawCount);
}

void Sprite::renderAtlas(
    Camera& c,
    Transform& t,
    Material& m,
    AtlasCoords& a,
    bool swapX,
    bool swapY)
{
    Program* program = AssetFactory::getInstance().getProgram("ATLAS");

    if (AssetFactory::getInstance().getActiveProgram() != program->getId()) {
        program->use();
        AssetFactory::getInstance().setActiveProgram(program->getId());
    }

    // set uniforms
    program->setUniform("camera", c.getViewProjectionMatrix());
    program->setUniform("model", t.getModelMatrix());
    program->setUniform("swapX", swapX);
    program->setUniform("swapY", swapY);

    program->setUniform("tex", 0); // texture will be bound to GL_TEXTURE0
    
    glActiveTexture(GL_TEXTURE0);
    
    if (AssetFactory::getInstance().getActiveTexture() != m.texture->getId()) {
        glBindTexture(GL_TEXTURE_2D, m.texture->getId());
        AssetFactory::getInstance().setActiveTexture(m.texture->getId());
    }

    bool useColor = false;
    if (m.color) {
        program->setUniform("color", *m.color);
        useColor = true;
    }

    program->setUniform("useColor", useColor);

    program->setUniform("atlasW", (float)m.texture->getWidth());
    program->setUniform("atlasH", (float)m.texture->getHeight());
    program->setUniform("spriteX", a.x);
    program->setUniform("spriteY", a.y);
    program->setUniform("spriteW", a.width);
    program->setUniform("spriteH", a.height);

    // set attributes
    glEnableVertexAttribArray(program->attrib("vert"));
    glVertexAttribPointer(program->attrib("vert"),
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        0);

    glEnableVertexAttribArray(program->attrib("vertTexCoord"));
    glVertexAttribPointer(program->attrib("vertTexCoord"),
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(GL_FLOAT),
        (const GLvoid*)(3 * sizeof(GLfloat)));

    glDrawArrays(drawType, drawStart, drawCount);
}