import {EffectComposer} from 'three/examples/jsm/postprocessing/EffectComposer.js';
import {RenderPass} from 'three/examples/jsm/postprocessing/RenderPass.js';
import {ShaderPass} from 'three/examples/jsm/postprocessing/ShaderPass.js';
import {FXAAShader} from 'three/examples/jsm/shaders/FXAAShader.js';
import * as THREE from "three";

export const addComposer = ({renderer, scene, camera}) => {
  const size = new THREE.Vector2();
  renderer.getSize(size);
  const {width, height} = size;
  const composer = new EffectComposer(renderer);
  const renderPass = new RenderPass(scene, camera);
  composer.addPass(renderPass);

  const fxaaPass = new ShaderPass(FXAAShader);
  fxaaPass.uniforms['resolution'].value.set(1 / width, 1 / height);
  composer.addPass(fxaaPass);

  return composer;
}
