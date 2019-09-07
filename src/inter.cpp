#include "inter.hpp"

Inter::Inter(int width, int height) : texture(), rbo(), fbo() {
  texture.bind(GL_TEXTURE_2D);
  texture.image2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, nullptr);
  texture.parameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  texture.parameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texture.unbind(GL_TEXTURE_2D);
  rbo.bind(GL_RENDERBUFFER);
  rbo.storage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  rbo.unbind(GL_RENDERBUFFER);
  fbo.bind(GL_FRAMEBUFFER);
  fbo.texture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture,
                0);
  fbo.renderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                   rbo);
  fbo.xassertComplete(GL_FRAMEBUFFER);
  fbo.unbind(GL_FRAMEBUFFER);
}

void Inter::bind() {
  fbo.bind(GL_FRAMEBUFFER);
  glEnable(GL_DEPTH_TEST);
}

void Inter::unbind() {
  glDisable(GL_DEPTH_TEST);
  fbo.unbind(GL_FRAMEBUFFER);
}
