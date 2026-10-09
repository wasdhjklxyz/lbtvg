// batman/, file unknown: inline virtuals of the level-object classes,
// emitted between 0x4f6c80 and 0x4f7530 (before LoadPerm2).

// The Mac names end in A: windows.h maps GetClassName to GetClassNameA.

struct InteractiveDisplay {
  const char *GetClassNameA() const;
};

struct WorldMapBase {
  const char *GetClassNameA() const;
};

struct SecurityCamera {
  const char *GetClassNameA() const;
};

struct LightFlickerOverlay {
  const char *GetClassNameA() const;
};

// FUNCTION: LEGOBATMAN 0x004f6ca0
const char *InteractiveDisplay::GetClassNameA() const {
  return "InteractiveDisplay";
}

// FUNCTION: LEGOBATMAN 0x004f6f70
const char *WorldMapBase::GetClassNameA() const { return "WorldMapBase"; }

// FUNCTION: LEGOBATMAN 0x004f73f0
const char *SecurityCamera::GetClassNameA() const { return "SecurityCamera"; }

// FUNCTION: LEGOBATMAN 0x004f74c0
const char *LightFlickerOverlay::GetClassNameA() const {
  return "LightFlickerOverlay";
}
