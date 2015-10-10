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

