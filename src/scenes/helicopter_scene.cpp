#include "helicopter_scene.hpp"
#include <glm/ext.hpp>
#include <vector>
#include <chrono>

static const std::vector<glm::vec4> CUBE = {
    {-.5, -.5, 1.0, .5},
    {.5, -.5, 1.0, .5},
    {.5, .5, 1.0, .5},
    {-.5, .5, 1.0, .5},
    {-.5, -.5, 1.0, -.5},
    {.5, -.5, 1.0, -.5},
    {.5, .5, 1.0, -.5},
    {-.5, .5, 1.0, -.5}
};

static const std::vector<GLuint> CUBEI = {
    0,1,2,
    2,3,0,
    0,4,3,
    3,4,7,
    7,4,5,
    5,6,7,
    7,3,2,
    2,6,7,
    0,1,4,
    4,1,5
};

static const std::vector<glm::vec4> OCTAHEDRON =
{
    {0.0, 0.0, -1.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {-1.0, 0.0, 0.0, 1.0},
    {0.0, -1.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0, 1.0}
};

static const std::vector<GLuint> OCTAHEDRONI = 
{
    2,5,1,
    1,5,4,
    4,5,3,
    3,5,2,
    2,3,0,
    0,2,1,
    1,0,4,
    4,0,3
};


static const std::vector<glm::vec4> BLADE = {
    {-.25, -1.0, 0.0, 1.0},
    {.25, -1.0, 0.0, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    {.25, 1.0, 0.0, 1.0},
    {-.25, 1.0, 0.0, 1.0},
};

static const std::vector<GLuint> BLADEI = {
    0,1,2,
    2,3,4,
};

void HelicopterScene::toggleView()
{
    if (mCurrentView == VIEW_INSIDE)
    {
        mView = glm::lookAt(glm::vec3(-2,2,-5), glm::vec3(0,0,0), glm::vec3(0,1,0));
        mCurrentView = VIEW_OUTSIDE;
    }
    else
    {
        mView = glm::lookAt(glm::vec3(0,.3,.35), glm::vec3(0,.4,1), glm::vec3(0,1,0));
        mCurrentView = VIEW_INSIDE;
    }
}

void HelicopterScene::step()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

    float dt = float(now - mLastFrameNs) * 0.000000001f;

    //mView = glm::lookAt(glm::vec3(-2,2,-5), glm::vec3(0,0,0), glm::vec3(0,1,0));
    //mView = glm::lookAt(glm::vec3(0,.3,.35), glm::vec3(0,.4,1), glm::vec3(0,1,0));

    if (mLastFrameNs <= 0)
        goto update;

    for (int i = 0; i < 3; ++i)
    {
        mAngles[i] += mAngularV[i] * dt;
        if (mAngles[i] >= glm::two_pi<float>())
            mAngles[i] -= glm::two_pi<float>();
        else if (mAngles[i] <= -glm::two_pi<float>())
            mAngles[i] += glm::two_pi<float>();
    }
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_HELI_MVP]);

    glm::mat4* heliMVP= (glm::mat4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    heliMVP[0] = mProjection * mView;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_BLADES_MVP]);

    glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0,.65,0));
    model = glm::rotate(model, mAngles[0], glm::vec3(0, 1, 0));
    model = glm::rotate(model, .5f*glm::pi<float>(), glm::vec3(1, 0, 0));
    model = glm::scale(model, glm::vec3(2));

    glm::mat4* bladeMVP = (glm::mat4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, 2*sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    bladeMVP[0] = mProjection * mView * model;

    model = glm::translate(glm::mat4(1), glm::vec3(-.1,.2,-2.4));
    model = glm::rotate(model, mAngles[1], glm::vec3(1, 0, 0));
    model = glm::rotate(model, .5f*glm::pi<float>(), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(.5));

    bladeMVP[1] = mProjection * mView * model;

    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_CUBE_MVP]);
    glm::mat4* cubeMVP = (glm::mat4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    model = glm::translate(glm::mat4(1), glm::vec3(1, 1, 5));
    model = glm::rotate(model, mAngles[2], glm::vec3(0, 1, 0));
    cubeMVP[0] = mProjection * mView * model;
    glUnmapBuffer(GL_ARRAY_BUFFER);

update:
    mLastFrameNs = now;
}

void HelicopterScene::OnStartGraphics()
{
    mCurrentView = VIEW_OUTSIDE;
    std::vector<std::pair <GLenum, std::string >> shaderFilenames = {{GL_VERTEX_SHADER, "../src/shaders/vertex.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "../src/shaders/fragment.glsl"}};

    mShader = new Shader(shaderFilenames);

    mAngularV[0] = 13.33f;
    mAngularV[1] = 40.0f;
    mAngularV[2] = 1.0f;

    for (int i = 0; i < 6; ++i)
        mBladeNorms[i] = glm::vec3(.5);

    updateProjectionMatrix();

    mHelicopterMesh = new Model("../src/test/helicopter_model_3.obj");

    glGenVertexArrays(VA_COUNT, mVA);
    glGenBuffers(VB_COUNT, mVB);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVA[VA_HELI]);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_HELI]);
    glBufferData(GL_ARRAY_BUFFER, mHelicopterMesh->meshes_.back().vertices_.size() * sizeof(Mesh::Vertex), &(mHelicopterMesh->meshes_.back().vertices_)[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mHelicopterMesh->meshes_[0].indices_.size()*sizeof(GLuint), &mHelicopterMesh->meshes_[0].indices_[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid*)offsetof(Mesh::Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_HELI_MVP]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(2+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(2+i);
        glVertexAttribDivisor(2+i, 1);
    }

    glBindVertexArray(0);

    glBindVertexArray(mVA[VA_BLADES]);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_BLADES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*BLADE.size(), &BLADE[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_BLADE_NORMS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*2, &mBladeNorms[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_BLADES_MVP]);
    glBufferData(GL_ARRAY_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(2+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(2+i);
        glVertexAttribDivisor(2+i, 1);
    }

    glBindVertexArray(0);

    glBindVertexArray(mVA[VA_CUBE]);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_CUBE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*OCTAHEDRON.size(), &OCTAHEDRON[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_CUBE_NORMS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*2, &mBladeNorms[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_CUBE_MVP]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(2+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(2+i);
        glVertexAttribDivisor(2+i, 1);
    }

    glBindVertexArray(0);

    //glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
    //glDepthFunc(GL_LEQUAL);
    //glDepthRange(0.0f, 1.0f);
}

void HelicopterScene::DoFrame()
{
    glClearColor(.2f, .2f, .2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(mShader->program_);
    step();
    glBindVertexArray(mVA[VA_HELI]);
    glDrawElements(GL_TRIANGLES, mHelicopterMesh->meshes_[0].indices_.size(), GL_UNSIGNED_INT, 0);
    //glDrawElementsInstanced(GL_TRIANGLES, HELICOPTERI.size(), GL_UNSIGNED_INT, &HELICOPTERI[0], 1);

    glBindVertexArray(mVA[VA_BLADES]);
    glDrawElementsInstanced(GL_TRIANGLES, BLADEI.size(), GL_UNSIGNED_INT, &BLADEI[0], 2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(mVA[VA_CUBE]);
    glDrawElements(GL_TRIANGLES, OCTAHEDRONI.size(), GL_UNSIGNED_INT, &OCTAHEDRONI[0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void HelicopterScene::updateProjectionMatrix()
{
    SceneManager* mgr = SceneManager::getInstance();
    mProjection = glm::perspective(glm::radians(60.0f), mgr->getScreenAspect(), 0.1f, 100.0f);
}
