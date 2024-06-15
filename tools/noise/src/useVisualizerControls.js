import {useEffect, useRef, useState} from "react";
import {Vector3} from "three";

export const useVisualizerControls = (cameraRef) => {
  const [pressedKeys, setPressedKeys] = useState(new Set());
  const animationRef = useRef(null);

  useEffect(() => {
    const handleKeyDown = (event) => {
      setPressedKeys((prevKeys) => new Set(prevKeys).add(event.key));
    };

    const handleKeyUp = (event) => {
      setPressedKeys((prevKeys) => {
        const newKeys = new Set(prevKeys);
        newKeys.delete(event.key);
        return newKeys;
      });
    };

    window.addEventListener('keydown', handleKeyDown);
    window.addEventListener('keyup', handleKeyUp);

    return () => {
      window.removeEventListener('keydown', handleKeyDown);
      window.removeEventListener('keyup', handleKeyUp);
    };
  }, []);

  useEffect(() => {
    const moveCamera = () => {
      const camera = cameraRef.current;
      if (!camera) {
        return;
      }
      const wPressed = pressedKeys.has('w');
      const sPressed = pressedKeys.has('s');
      const aPressed = pressedKeys.has('a');
      const dPressed = pressedKeys.has('d');

      if (!(wPressed || sPressed || aPressed || dPressed)) {
        return;
      }

      const direction = new Vector3();
      const strafeDirection = new Vector3();
      const moveDistance = 1;

      camera.getWorldDirection(direction);
      strafeDirection.crossVectors(camera.up, direction).normalize();

      if (wPressed) {
        camera.position.add(direction.multiplyScalar(moveDistance));
      }
      if (sPressed) {
        camera.position.add(direction.multiplyScalar(-moveDistance));
      }
      if (aPressed) {
        camera.position.add(strafeDirection.multiplyScalar(moveDistance));
      }
      if (dPressed) {
        camera.position.add(strafeDirection.multiplyScalar(-moveDistance));
      }

      animationRef.current = requestAnimationFrame(moveCamera);
    };

    animationRef.current = requestAnimationFrame(moveCamera);

    return () => {
      cancelAnimationFrame(animationRef.current);
    };
  }, [pressedKeys]);
};