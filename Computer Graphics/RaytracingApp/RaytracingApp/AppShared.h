#define _CRT_SECURE_NO_WARNINGS
#include "directxtk/SimpleMath.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

struct Vec4;
struct Vertex;
class Timer;

#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <iostream>
#include <string>
#include <omp.h>
#include <algorithm>
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <math.h>
#include "Vertex.h"
#include "Timer.h"
#include "Utils.h"
#include "BaseApp.h"

#include "Constant.h"
#include "Image.h"
#include "Model.h"
#include "Circle.h"
#include "RenderApp.h"






