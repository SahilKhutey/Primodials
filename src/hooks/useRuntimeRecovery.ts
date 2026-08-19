import { useEffect, useRef, useState } from "react";
import type { Simulation } from "@/sim/simulation";
import { installCrashRecovery, previousRunWasUnclean, recoverPreviousRun } from "@/lib/crashRecovery";

export function useRuntimeRecovery(sim: Simulation) {
  const cleanupRef = useRef<(() => void) | null>(null);
  const [uncleanPreviousRun, setUncleanPreviousRun] = useState(false);
  const [recovered, setRecovered] = useState(false);

  useEffect(() => {
    setUncleanPreviousRun(previousRunWasUnclean());
    cleanupRef.current = installCrashRecovery(sim);

    return () => {
      cleanupRef.current?.();
      cleanupRef.current = null;
    };
  }, [sim]);

  const recover = () => {
    const restored = recoverPreviousRun();
    if (!restored) return null;
    setRecovered(true);
    return restored;
  };

  return {
    uncleanPreviousRun,
    recovered,
    recover,
  };
}
