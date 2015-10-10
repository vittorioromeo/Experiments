
/*
foreach(entity/object drawable)
{
    // Bind VAO & Shader
    // glDrawArrays / glDrawElements
    // Unbind VAO & Shader
}
*/

/*
foreach(vertexarray draw call)
{
    apply transform;
    apply view;
    apply blend mode;
    apply texture;

    apply shader;
        use program; (stores a ptr to it)
        bind textures;

        un-use program;

    glVertexPointer;
        glCheck(glVertexPointer(2, GL_FLOAT, sizeof(Vertex), data + 0));

    glColorPointer;
        glCheck(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), data + 8));

    glTexCoordPointer;
        glCheck(glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), data + 12));

    glDrawArrays;

    unbind shader;
}
*/

/*
    //initialization
    glGenVertexArrays
    glBindVertexArray

    glGenBuffers
    glBindBuffer
    glBufferData

    glVertexAttribPointer
    glEnableVertexAttribArray

    glBindVertexArray(0)

    glDeleteBuffers //you can already delete it after the VAO is
    unbound, since the
                     //VAO still references it, keeping it alive (see
    comments below).

    ...

    //rendering
    glBindVertexArray
    glDrawWhatever
*/
    


// Init

// vao->bind
glGenVertexArrays(1, &m_id);
glGenBuffers(1, &m_id);
glBindVertexArray(m_id);

// vbo->bind
glBindBuffer(GL_ARRAY_BUFFER, m_id);
glBufferData(GL_ARRAY_BUFFER, max_size, nullptr, GL_DYNAMIC_DRAW);

glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
glEnableVertexAttribArray(SHADER_UV_INDEX);
glEnableVertexAttribArray(SHADER_COLOR_INDEX);

glVertexAttribPointer(
    SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    reinterpret_cast<const GLvoid*>(offsetof(Vertex, uv)));

glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
    reinterpret_cast<const GLvoid*>(offsetof(Vertex, color)));

// vbo->unbind
glBindBuffer(GL_ARRAY_BUFFER, 0);



// Draw

// vao->bind
glBindVertexArray(m_id);
m_buffer = static_cast<Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

m_buffer->position = position;
m_buffer->color = color;
m_buffer++;

m_buffer->position = position1;
m_buffer->color = color1;
m_buffer++;

glUnmapBuffer(GL_ARRAY_BUFFER);
glDrawElements(...);

// vao->unbind
glBindVertexArray(0);

