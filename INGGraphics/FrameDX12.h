//----------------------------------------------------------------------------------
// FrameDX12.h : Definition the ING Graphics frame for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <IFrame.h>

namespace ING {
namespace Graphics {

    class FrameDX12 : public IFrame
    {
    public:
        FrameDX12(ID3D12Device *pD3DDevice);
        virtual ~FrameDX12();

        virtual result_code_t OnSizeChanged(const int width, const int height);
    private:
    };

}}
