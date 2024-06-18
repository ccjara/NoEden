import { useCallback, useEffect, useMemo, useRef } from 'react';
import * as THREE from 'three';
import { addComposer } from './addComposer';
import { useVisualizerControls } from './useVisualizerControls';
import { useStore } from './store/store.js';
import { createHeightColorizer } from './height/createHeightColorizer.js';

const V_WIDTH = 1280;
const V_HEIGHT = 720;

export const Visualizer = () => {
  const { width, height } = useStore(($) => $.mapSize);
  const data = useStore(($) => $.height.map);
  const altitudeOptions = useStore(($) => $.height.options.altitude);

  const mountRef = useRef(null);
  const sceneRef = useRef(null);
  const cameraRef = useRef(null);
  const rendererRef = useRef(null);
  const instancedMeshRef = useRef(null);
  const directionalLightRef = useRef(null);
  const blockMaterialRef = useRef(null);

  useVisualizerControls(cameraRef);

  const colorize = useMemo(
    () => createHeightColorizer(altitudeOptions),
    [altitudeOptions],
  );

  const onMouseMove = (event) => {
    if (!cameraRef.current || event.buttons === 0) {
      return;
    }
    const rotationSpeed = 0.002;
    const euler = new THREE.Euler(0, 0, 0, 'YXZ');

    euler.setFromQuaternion(cameraRef.current.quaternion);

    euler.y -= event.movementX * rotationSpeed;
    euler.x -= event.movementY * rotationSpeed;
    euler.x = Math.max(-Math.PI / 2, Math.min(Math.PI / 2, euler.x));

    cameraRef.current.quaternion.setFromEuler(euler);
  };

  useEffect(() => {
    if (!mountRef.current || sceneRef.current) {
      return;
    }

    const scene = new THREE.Scene();
    const camera = new THREE.PerspectiveCamera(75, V_WIDTH / V_HEIGHT);
    const renderer = new THREE.WebGLRenderer();
    blockMaterialRef.current = createShaderMaterial();

    renderer.setSize(V_WIDTH, V_HEIGHT);

    const composer = addComposer({ renderer, scene, camera });

    mountRef.current.appendChild(renderer.domElement);

    const ambientLight = new THREE.AmbientLight(0xffffff);
    scene.add(ambientLight);

    const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
    directionalLight.position.set(0, 8, 0).normalize();
    directionalLightRef.current = directionalLight;
    scene.add(directionalLight);

    sceneRef.current = scene;
    cameraRef.current = camera;
    rendererRef.current = renderer;

    const camera_offset = (width + height) / 2;
    camera.position.set(camera_offset, camera_offset, camera_offset);
    camera.lookAt(new THREE.Vector3(0, 0, 0));

    const animate = () => {
      requestAnimationFrame(animate);
      renderer.render(scene, camera);
      composer.render();
    };
    animate();
  }, []);

  useEffect(() => {
    if (!sceneRef.current) {
      return;
    }
    const scene = sceneRef.current;
    const material = blockMaterialRef.current;

    // remove instanced mesh
    if (instancedMeshRef.current) {
      scene.remove(instancedMeshRef.current);
    }

    const instanceCount = width * height;
    const colors = new Float32Array(instanceCount * 4);
    const geometry = new THREE.BoxGeometry(1, 1, 1);
    const instancedMesh = new THREE.InstancedMesh(
      geometry,
      material,
      instanceCount,
    );
    const matrix = new THREE.Matrix4();

    for (let z = 0; z < height; ++z) {
      for (let x = 0; x < width; ++x) {
        const index = z * width + x;
        colors.set(
          colorize(data[index]).map(($) => $ / 255),
          index * 4,
        );

        const blockHeight = data[index];

        matrix.makeScale(1, 1, 1);
        matrix.setPosition(x, blockHeight / 2, z);
        instancedMesh.setMatrixAt(z * height + x, matrix);
      }
    }
    geometry.setAttribute(
      'instanceColor',
      new THREE.InstancedBufferAttribute(colors, 4),
    );

    scene.add(instancedMesh);
    instancedMeshRef.current = instancedMesh;
  }, [data, width, height]);

  return <div ref={mountRef} onMouseMove={onMouseMove} />;
};

function createShaderMaterial() {
  return new THREE.ShaderMaterial({
    vertexShader: `
      attribute vec4 instanceColor;
      varying vec4 vColor;
      void main() {
        vColor = instanceColor;
        vec4 mvPosition = modelViewMatrix * instanceMatrix * vec4(position, 1.0);
        gl_Position = projectionMatrix * mvPosition;
      }
    `,
    fragmentShader: `
      varying vec4 vColor;
      void main() {
        gl_FragColor = vColor;
      }
    `,
    vertexColors: true,
  });
}
