#include "test_scene.hpp"
#include "../src/gizmos/shapes.hpp"

TestScene::TestScene() : mLastFrameNs(0), Scene() 
{
    mShader = NULL;

    mView = glm::lookAt(glm::vec3(0.0f, 2.0f, 50.0f), glm::vec3(0.0f,10.0f,0.0f), glm::vec3(0,1,0));
    updateProjectionMatrix(10.0f);

    mNumInstances = 3000;
    mTimeCount = 0;
}

void TestScene::OnStartGraphics()
{
    mTimeCount = 0;
    LOGI("Starting Scene...\n");
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::vector<std::pair <GLenum, std::string >> shaderFilenames = {{GL_VERTEX_SHADER, "shaders/test_scene.vs.glsl"},
                                                                     {GL_FRAGMENT_SHADER, "shaders/test_scene.fs.glsl"}};

    mShader = new gl00::Shader(shaderFilenames);

    glGenBuffers(VB_COUNT, mVB);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(OCTAHEDRON), &OCTAHEDRON[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_MVP]);
    glBufferData(GL_ARRAY_BUFFER, mNumInstances * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, pos));
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_MVP]);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribPointer(1+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
        glEnableVertexAttribArray(1+i);
        glVertexAttribDivisor(1+i, 1);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}

void TestScene::updateProjectionMatrix()
{
    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();
    mProjection = glm::perspective(glm::radians(45.0f), mgr->GetScreenAspect(), 0.1f, 100.0f);
}

void TestScene::updateProjectionMatrix(float a)
{
    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();
    mProjection = glm::perspective(glm::radians(a), mgr->GetScreenAspect(), 0.1f, 100.0f);
}

void TestScene::OnScreenResize(int width, int height)
{
    gl00::SceneManager* mgr = gl00::SceneManager::GetInstance();
    mgr->SetScreenSize(width, height);
    glViewport(0, 0, width, height);
}

void TestScene::step()
{
    uint64_t now = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

    float dt = float(now - mLastFrameNs) * 0.000000001f;
    
    if (dt < 1)
        mTimeCount += dt;

    float a = 90.0f-40.0f*(1 + glm::sin(mTimeCount*.08f));
    updateProjectionMatrix(a);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_MVP]);

    glm::mat4* offset = (glm::mat4*)glMapBufferRange(GL_ARRAY_BUFFER, 0, mNumInstances * sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    glm::mat4 t;
    glm::mat4 r;
    glm::mat4 s;
    glm::mat4 u = glm::rotate(glm::mat4(1.0f), glm::cos(mTimeCount*.1f)*2*glm::pi<float>(), glm::vec3(0, 1, 0));
    glm::mat4 viewScale = glm::scale(glm::mat4(1.0f), glm::vec3(mViewScale));

    int numPerRow = 40;

    for (int i = 0; i < mNumInstances; i++)
    {
        t = glm::translate(glm::mat4(1.0f), glm::vec3(2*(i/numPerRow)-90.0f, (i%numPerRow)*2-20.0f, 20.0f*(1+glm::sin(mTimeCount+.01f*i))));
        r = glm::rotate(glm::mat4(1.0f), (0.5f+.5f*glm::cos(mTimeCount+i*0.1f))*2.0f*glm::pi<float>(), glm::vec3(0.0f, 1.0f, 1.0f));
        s = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1.25f-(0.5f + .5f*glm::cos(mTimeCount + i * 0.09f))));

        offset[i] = mProjection * mView * viewScale * u * t * r * s;
    }

    glUnmapBuffer(GL_ARRAY_BUFFER);
    mLastFrameNs = now;
}

void TestScene::DoFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    step();

    glUseProgram(mShader->program_);
    glBindVertexArray(mVAO);

    //glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, mNumInstances);
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_INT, &OCTAHEDRONI[0], mNumInstances);
}
