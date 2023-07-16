#ifndef MENU
#define MENU

#include <map>
#include "Data/Fonts/Minecraftia.h"
#include "Data/Fonts/combo.h"

static ImVec4 clr = ImVec4(0.36f, 0.24f, 0.82f, 1.f);

namespace Test {

    ImFont* combo_arrow;
    ImFont* fnt;
    void initFont(ImGuiIO& io)
    {
        Test::combo_arrow = io.Fonts->AddFontFromMemoryTTF(combo, 9, 9.0f);
        Test::fnt = io.Fonts->AddFontFromMemoryTTF(Minecraftia, 30, 30.f);
    }
    // ColorEdit supports RGB and HSV inputs. In case of RGB input resulting color may have undefined hue and/or saturation.
    // Since widget displays both RGB and HSV values we must preserve hue and saturation to prevent these values resetting.
    static void ColorEditRestoreHS(const float *col, float *H, float *S, float *V) {
        // This check is optional. Suppose we have two color widgets side by side, both widgets display different colors, but both colors have hue and/or saturation undefined.
        // With color check: hue/saturation is preserved in one widget. Editing color in one widget would reset hue/saturation in another one.
        // Without color check: common hue/saturation would be displayed in all widgets that have hue/saturation undefined.
        // g.ColorEditLastColor is stored as ImU32 RGB value: this essentially gives us color equality check with reduced precision.
        // Tiny external color changes would not be detected and this check would still pass. This is OK, since we only restore hue/saturation _only_ if they are undefined,
        // therefore this change flipping hue/saturation from undefined to a very tiny value would still be represented in color picker.
        ImGuiContext &g = *GImGui;
        float idk[3] = {col[0], col[1], col[2]};
        if (g.ColorEditLastColor != idk)
            return;

        // When S == 0, H is undefined.
        // When H == 1 it wraps around to 0.
        if (*S == 0.0f || (*H == 0.0f && g.ColorEditLastHue == 1))
            *H = g.ColorEditLastHue;

        // When V == 0, S is undefined.
        if (*V == 0.0f)
            *S = g.ColorEditLastSat;
    }

    bool SideBtn(const char *label, ImVec2 size_arg = ImVec2(0.f, 0.f)) {
        ImGuiButtonFlags flags = 0;
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y <
                                                            window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
            pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f,
                                          label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ImGui::ItemSize(size, style.FramePadding.y);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

        // Render
        const ImU32 col = ImGui::GetColorU32(ImVec4(clr.x, clr.y, clr.z, 1.f));
        ImGui::RenderNavHighlight(bb, id);
        ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

        if (g.LogEnabled)
            ImGui::LogSetNextTextDecoration("[", "]");
        ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label,
                                 NULL, &label_size, style.ButtonTextAlign, &bb);

        // Automatically close popups
        //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
        //    CloseCurrentPopup();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed;
    }

    struct checkbox_animation {
        float animation;
    };

    bool Checkbox(const char* label, bool* v)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

        const float w = ImGui::GetWindowWidth() - 30;
        const float square_sz_x = label_size.y + 70;
        const float square_sz = label_size.y + 30;
        const ImVec2 pos = window->DC.CursorPos;
        const ImRect frame_bb(pos, window->DC.CursorPos + ImVec2(label_size.x + square_sz_x + 19, square_sz - 1));
        const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
        ImGui::ItemSize(total_bb, style.FramePadding.y);
        if (!ImGui::ItemAdd(total_bb, id))
        {
            IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
            return false;
        }

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held);
        if (pressed)
        {
            *v = !(*v);
            ImGui::MarkItemEdited(id);
        }

        static std::map <ImGuiID, checkbox_animation> anim;
        auto it_anim = anim.find(id);
        if (it_anim == anim.end())
        {
            anim.insert({ id, { 0.0f } });
            it_anim = anim.find(id);
        }

        it_anim->second.animation = ImLerp(it_anim->second.animation, *v ? 1.0f : 0.0f, 0.12f * (1.0f - ImGui::GetIO().DeltaTime));

        ImGui::RenderNavHighlight(total_bb, id);

        ImGui::RenderFrame(frame_bb.Min, pos + ImVec2(square_sz_x + 13, square_sz), ImColor(15, 15, 16), false, 9.0f);
        ImGui::RenderFrame(frame_bb.Min, pos + ImVec2(square_sz_x + 13, square_sz), ImColor(clr.x, clr.y, clr.z, it_anim->second.animation), false, 9.0f);

        window->DrawList->AddCircleFilled(ImVec2(frame_bb.Min.x + 25 + 66 * it_anim->second.animation, frame_bb.Min.y + (square_sz / 2)), 15.0f, ImColor(1.0f, 1.0f, 1.0f), 30);

        if (label_size.x > 0.0f)
            ImGui::RenderText(total_bb.Min + ImVec2(square_sz_x + 24, (label_size.y/2)), label);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return pressed;
    }

    bool ColorPicker(const char *label, float col[4],
                     ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs) {
        using namespace ImGui;
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImVec2 text_size = CalcTextSize(label, NULL, true);
        const ImVec2 square_sz = text_size + ImVec2(15.f, 15.f);
        const float w_full = CalcItemWidth();
        const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz.x +
                                                                                      style.ItemInnerSpacing.x);
        const float w_inputs = w_full - w_button;
        const char *label_display_end = FindRenderedTextEnd(label);
        g.NextItemData.ClearFlags();

        BeginGroup();
        PushID(label);

        // If we're not showing any slider there's no point in doing any HSV conversions
        const ImGuiColorEditFlags flags_untouched = flags;
        if (flags & ImGuiColorEditFlags_NoInputs)
            flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB |
                    ImGuiColorEditFlags_NoOptions;

        // Context menu: display and modify options (before defaults are applied)
        if (!(flags & ImGuiColorEditFlags_NoOptions))
            ColorEditOptionsPopup(col, flags);

        // Read stored options
        if (!(flags & ImGuiColorEditFlags_DisplayMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
        if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
        if (!(flags & ImGuiColorEditFlags_PickerMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
        if (!(flags & ImGuiColorEditFlags_InputMask_))
            flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
        flags |= (g.ColorEditOptions &
                  ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ |
                    ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
        IM_ASSERT(ImIsPowerOfTwo(
            flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
        IM_ASSERT(ImIsPowerOfTwo(
            flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

        const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
        const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
        const int components = alpha ? 4 : 3;

        // Convert to the formats we need
        float f[4] = {col[0], col[1], col[2], alpha ? col[3] : 1.0f};
        if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
            ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
        else if ((flags & ImGuiColorEditFlags_InputRGB) &&
                 (flags & ImGuiColorEditFlags_DisplayHSV)) {
            // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
        }
        int i[4] = {IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]),
                    IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3])};

        bool value_changed = false;
        bool value_changed_as_float = false;

        const ImVec2 pos = window->DC.CursorPos;
        const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button
                                                                                   : 0.0f;
        window->DC.CursorPos.x = pos.x + inputs_offset_x;

        if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 &&
            (flags & ImGuiColorEditFlags_NoInputs) == 0) {
            // RGB/HSV 0..255 Sliders
            const float w_item_one = ImMax(1.0f, IM_FLOOR(
                (w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) /
                (float) components));
            const float w_item_last = ImMax(1.0f, IM_FLOOR(
                w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

            const bool hide_prefix = (w_item_one <= CalcTextSize(
                (flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
            static const char *ids[4] = {"##X", "##Y", "##Z", "##W"};
            static const char *fmt_table_int[3][4] =
                {
                    {"%3d",   "%3d",   "%3d",   "%3d"}, // Short display
                    {"R:%3d", "G:%3d", "B:%3d", "A:%3d"}, // Long display for RGBA
                    {"H:%3d", "S:%3d", "V:%3d", "A:%3d"}  // Long display for HSVA
                };
            static const char *fmt_table_float[3][4] =
                {
                    {"%0.3f",   "%0.3f",   "%0.3f",   "%0.3f"}, // Short display
                    {"R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f"}, // Long display for RGBA
                    {"H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f"}  // Long display for HSVA
                };
            const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

            for (int n = 0; n < components; n++) {
                if (n > 0)
                    SameLine(0, style.ItemInnerSpacing.x);
                SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

                // FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
                if (flags & ImGuiColorEditFlags_Float) {
                    value_changed |= DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f,
                                               hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
                    value_changed_as_float |= value_changed;
                } else {
                    value_changed |= DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255,
                                             fmt_table_int[fmt_idx][n]);
                }
                if (!(flags & ImGuiColorEditFlags_NoOptions))
                    OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
            }
        } else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 &&
                   (flags & ImGuiColorEditFlags_NoInputs) == 0) {
            // RGB Hexadecimal Input
            char buf[64];
            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255),
                               ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255),
                               ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
            SetNextItemWidth(w_inputs);
            window->DC.CursorPos = {pos.x - 30, pos.y};
            PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));
            if (InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal |
                                                            ImGuiInputTextFlags_CharsUppercase)) {
                value_changed = true;
                char *p = buf;
                while (*p == '#' || ImCharIsBlankA(*p))
                    p++;
                i[0] = i[1] = i[2] = 0;
                i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
                int r;
                if (alpha)
                    r = sscanf(p, "%02X%02X%02X%02X", (unsigned int *) &i[0],
                               (unsigned int *) &i[1], (unsigned int *) &i[2],
                               (unsigned int *) &i[3]); // Treat at unsigned (%X is unsigned)
                else
                    r = sscanf(p, "%02X%02X%02X", (unsigned int *) &i[0], (unsigned int *) &i[1],
                               (unsigned int *) &i[2]);
                IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
            }
            PopStyleColor();
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
        }

        ImGuiWindow *picker_active_window = NULL;
        if (!(flags & ImGuiColorEditFlags_NoSmallPreview)) {
            const float button_offset_x = window->Size.x - 50;
            window->DC.CursorPos = ImVec2(pos.x, pos.y - 3);

            const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
            if (ColorButton("##ColorButton", col_v4, flags, square_sz - ImVec2(20.f, 0.f))) {
                if (!(flags & ImGuiColorEditFlags_NoPicker)) {
                    // Store current color and open a picker
                    g.ColorPickerRef = col_v4;
                    OpenPopup("picker");
                    SetNextWindowPos(
                        g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
                }
            }
            if (!(flags & ImGuiColorEditFlags_NoOptions))
                OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

            PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
            if (BeginPopup("picker")) {
                picker_active_window = g.CurrentWindow;
                ImGuiColorEditFlags picker_flags_to_forward =
                    ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ |
                    ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR |
                    ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
                ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) |
                                                   ImGuiColorEditFlags_DisplayMask_ |
                                                   ImGuiColorEditFlags_NoLabel |
                                                   ImGuiColorEditFlags_AlphaPreviewHalf |
                                                   ImGuiColorEditFlags_NoSidePreview |
                                                   ImGuiColorEditFlags_NoInputs;
                SetNextItemWidth(square_sz.x * 9.0f); // Use 256 + bar sizes?
                value_changed |= ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
                EndPopup();
            }
            PopStyleVar();
        }

        if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel)) {
            window->DC.CursorPos = ImVec2(pos.x + square_sz.x + 5.f, pos.y + 2.f);
            TextEx(label, label_display_end);
        }


        // Convert back
        if (value_changed && picker_active_window == NULL) {
            if (!value_changed_as_float)
                for (int n = 0; n < 4; n++)
                    f[n] = i[n] / 255.0f;
            if ((flags & ImGuiColorEditFlags_DisplayHSV) &&
                (flags & ImGuiColorEditFlags_InputRGB)) {
                g.ColorEditLastHue = f[0];
                g.ColorEditLastSat = f[1];
                ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
                g.ColorEditLastColor[0] = f[0];
                g.ColorEditLastColor[1] = f[1];
                g.ColorEditLastColor[2] = f[2];
            }
            if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

            col[0] = f[0];
            col[1] = f[1];
            col[2] = f[2];
            if (alpha)
                col[3] = f[3];
        }

        PopID();
        EndGroup();

        // Drag and Drop Target
        // NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
        if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) &&
            !(flags & ImGuiColorEditFlags_NoDragDrop) && BeginDragDropTarget()) {
            bool accepted_drag_drop = false;
            if (const ImGuiPayload *payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F)) {
                memcpy((float *) col, payload->Data,
                       sizeof(float) * 3); // Preserve alpha if any //-V512
                value_changed = accepted_drag_drop = true;
            }
            if (const ImGuiPayload *payload = AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F)) {
                memcpy((float *) col, payload->Data, sizeof(float) * components);
                value_changed = accepted_drag_drop = true;
            }

            // Drag-drop payloads are always RGB
            if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
                ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
            EndDragDropTarget();
        }

        // When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
        if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
            g.LastItemData.ID = g.ActiveId;

        if (value_changed)
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    struct slider_element {
        float opacity;
    };
    void ImFormatStringToTempBuffer(const char** out_buf, const char** out_buf_end, const char* fmt, ...)
    {
        ImGuiContext& g = *GImGui;
        va_list args;
        va_start(args, fmt);
        if (fmt[0] == '%' && fmt[1] == 's' && fmt[2] == 0)
        {
            const char* buf = va_arg(args, const char*); // Skip formatting when using "%s"
            *out_buf = buf;
            if (out_buf_end) { *out_buf_end = buf + strlen(buf); }
        }
        else
        {
            int buf_len = ImFormatStringV(g.TempBuffer, sizeof(g.TempBuffer), fmt, args);
            *out_buf = g.TempBuffer;
            if (out_buf_end) { *out_buf_end = g.TempBuffer + buf_len; }
        }
        va_end(args);
    }

    static const char* PatchFormatStringFloatToInt(const char* fmt)
    {
        if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
            return "%d";
        const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
        const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
        if (fmt_end > fmt_start && fmt_end[-1] == 'f')
        {
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
            if (fmt_start == fmt && fmt_end[0] == 0)
                return "%d";
            const char* tmp_format;
            ImFormatStringToTempBuffer(&tmp_format, NULL, "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
            return tmp_format;
#else
            IM_ASSERT(0 && "DragInt(): Invalid format string!"); // Old versions used a default parameter of "%.0f", please replace with e.g. "%d"
#endif
        }
        return fmt;
    }

    bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
    {
        using namespace ImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const float w = GetWindowWidth() - 100.f;

        const ImVec2 label_size = CalcTextSize(label, NULL, true);
        const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + 30));
        const ImRect frame_bb(total_bb.Min + ImVec2(0, label_size.y + 5), total_bb.Max);

        ItemSize(total_bb, style.FramePadding.y);
        if (!ItemAdd(total_bb, id, &frame_bb))
            return false;

        // Default format string when passing NULL
        if (format == NULL)
            format = DataTypeGetInfo(data_type)->PrintFmt;
        else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
            format = PatchFormatStringFloatToInt(format);

        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool hovered = ItemHoverable(frame_bb, id);
        const bool clicked = (hovered && g.IO.MouseClicked[0]);
        if (clicked)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }

        static std::map <ImGuiID, slider_element> anim;
        auto it_anim = anim.find(id);
        if (it_anim == anim.end())
        {
            anim.insert({ id, { 0.0f } });
            it_anim = anim.find(id);
        }

        it_anim->second.opacity = ImLerp(it_anim->second.opacity, IsItemActive() ? 1.0f : 0.4f, 0.08f * (1.0f - ImGui::GetIO().DeltaTime));

        ImRect grab_bb;
        const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
        if (value_changed)
            MarkItemEdited(id);

        char value_buf[64];
        const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImColor(32, 33, 35), 5.0f);
        window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(grab_bb.Min.x + 5, frame_bb.Max.y),
                                        GetColorU32(ImVec4(clr.x, clr.y, clr.z, 1.f)), 5.0f);

        RenderText(total_bb.Min, label);

        PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, it_anim->second.opacity)); {
            RenderTextClipped(total_bb.Min, total_bb.Max, value_buf, value_buf_end, NULL, ImVec2(1.f, 0.f));
        } PopStyleColor();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
        return value_changed;
    }

    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
    {
        return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
    }

    static float CalcMaxPopupHeightFromItemCount(int items_count)
    {
        ImGuiContext& g = *GImGui;
        if (items_count <= 0)
            return FLT_MAX;
        return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
    }

    struct combo_element {
        float open_anim, arrow_anim;
    };

    struct selectable_element {
        float opacity;
    };

    bool Selectable(const char* label, bool selected, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0, 0))
    {
        using namespace ImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
        ImGuiID id = window->GetID(label);
        ImVec2 label_size = CalcTextSize(label, NULL, true);
        ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
        ImVec2 pos = window->DC.CursorPos;
        pos.y += window->DC.CurrLineTextBaseOffset;
        ItemSize(size, 0.0f);

        // Fill horizontal space
        // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
        const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
            size.x = ImMax(label_size.x, max_x - min_x);

        // Text stays at the submission position, but bounding box may be extended on both sides
        const ImVec2 text_min = pos;
        const ImVec2 text_max(min_x + size.x, pos.y + size.y);

        // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
        ImRect bb(min_x, pos.y, text_max.x, text_max.y);
        if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
            const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += (spacing_x - spacing_L);
            bb.Max.y += (spacing_y - spacing_U);
        }
        //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

        // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if (span_all_columns)
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
        const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
        if (span_all_columns)
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if (!item_add)
            return false;

        const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
        if (disabled_item && !disabled_global) // Only testing this as an optimization
            PushDisabled(true);

        // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
        // which would be advantageous since most selectable are not selected.
        if (span_all_columns && window->DC.CurrentColumns)
            PushColumnsBackground();
        else if (span_all_columns && g.CurrentTable)
            TablePushBackgroundChannel();

        // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
        ImGuiButtonFlags button_flags = 0;
        if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
        if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
        if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
        if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }

        const bool was_selected = selected;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

        // Auto-select when moved into
        // - This will be more fully fleshed in the range-select branch
        // - This is not exposed as it won't nicely work with some user side handling of shift/control
        // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
        //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
        //   - (2) usage will fail with clipped items
        //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
        if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == window->DC.NavFocusScopeIdCurrent)
            if (g.NavJustMovedToId == id)
                selected = pressed = true;

        // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
        if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
        {
            if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
            {
                SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect(bb.Min - window->Pos, bb.Max - window->Pos)); // (bb == NavRect)
                g.NavDisableHighlight = true;
            }
        }
        if (pressed)
            MarkItemEdited(id);

        if (flags & ImGuiSelectableFlags_AllowItemOverlap)
            SetItemAllowOverlap();

        // In this branch, Selectable() cannot toggle the selection so this will never trigger.
        if (selected != was_selected) //-V547
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

        // Render
        if (held && (flags & ImGuiSelectableFlags_SetNavIdOnHover))
            hovered = true;

        RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

        static std::map <ImGuiID, selectable_element> anim;
        auto it_anim = anim.find(id);
        if (it_anim == anim.end()) {
            anim.insert({ id, { 0.0f } });
            it_anim = anim.find(id);
        }

        it_anim->second.opacity = ImLerp(it_anim->second.opacity, (selected ? 1.0f : hovered ? 0.5f : 0.2f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));

        if (span_all_columns && window->DC.CurrentColumns)
            PopColumnsBackground();
        else if (span_all_columns && g.CurrentTable)
            TablePopBackgroundChannel();

        PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, it_anim->second.opacity)); {
            RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
        } PopStyleColor();

        // Automatically close popups
        if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
            CloseCurrentPopup();

        if (disabled_item && !disabled_global)
            PushDisabled(true);

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed; //-V1020
    }

    bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
    {
        using namespace ImGui;
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();

        ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
        g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
        if (window->SkipItems)
            return false;

        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
        const float size = GetWindowWidth() - 30;

        const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size, 53));
        const ImRect clickable(window->DC.CursorPos + ImVec2(0, 23), window->DC.CursorPos + ImVec2(size, 53));

        const ImVec2 arrow_size = combo_arrow->CalcTextSizeA(9.0f, FLT_MAX, 0.0f, "A");

        ItemSize(rect, style.FramePadding.y);
        if (!ItemAdd(clickable, id, &rect))
            return false;

        // Open on click
        bool hovered, held;
        bool pressed = ButtonBehavior(clickable, id, &hovered, &held);
        const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
        bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
        if (pressed && !popup_open)
        {
            OpenPopupEx(popup_id, ImGuiPopupFlags_None);
            popup_open = true;
        }

        static std::map <ImGuiID, combo_element> anim;
        auto it_anim = anim.find(id);
        if (it_anim == anim.end())
        {
            anim.insert({ id, { 0.0f, 0.0f } });
            it_anim = anim.find(id);
        }

        it_anim->second.open_anim = ImLerp(it_anim->second.open_anim, popup_open ? 1.0f : 0.0f, 0.04f * (1.0f - ImGui::GetIO().DeltaTime));
        it_anim->second.arrow_anim = ImLerp(it_anim->second.arrow_anim, popup_open ? 0.3f : 0.0f, 0.05f * (1.0f - ImGui::GetIO().DeltaTime));

        window->DrawList->AddRectFilled(clickable.Min, clickable.Max, ImColor(34, 34, 36), 3.0f);
        window->DrawList->AddRect(clickable.Min, clickable.Max, ImColor(1.0f, 1.0f, 1.0f, 0.03f), 3.0f);

        window->DrawList->AddText(rect.Min, GetColorU32(ImGuiCol_Text), label);

        RenderTextClipped(clickable.Min + ImVec2(14, 7), clickable.Max - ImVec2(24, -47), preview_value, NULL, NULL, ImVec2(0.0f, 0.0f));

        PushFont(combo_arrow); {
            window->DrawList->AddText(ImVec2(clickable.Max.x - 20, (clickable.Min.y + clickable.Max.y) / 2 - arrow_size.y / 2), ImColor(1.0f, 1.0f, 1.0f, 0.2f + it_anim->second.arrow_anim), "A");
        } PopFont();
        SetCurrentFont(fnt);

        if (!popup_open)
            return false;

        g.NextWindowData.Flags = backup_next_window_data_flags;
        if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
        {
            g.NextWindowData.ClearFlags();
            return false;
        }

        // Set popup size
        float w = clickable.GetWidth();
        if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
        {
            g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
        }
        else
        {
            if ((flags & ImGuiComboFlags_HeightMask_) == 0)
                flags |= ImGuiComboFlags_HeightRegular;
            IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
            int popup_max_height_in_items = -1;
            if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
            else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
            else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
            SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items) * it_anim->second.open_anim));
        }

        // This is essentially a specialized version of BeginPopupEx()
        char name[16];
        ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

        // Set position given a custom constraint (peak into expected window size so we can position it)
        // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
        // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
        if (ImGuiWindow* popup_window = FindWindowByName(name))
            if (popup_window->WasActive)
            {
                // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
                ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
                popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
                ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(clickable.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, clickable, ImGuiPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos + ImVec2(0, 3));
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 5)); // Horizontally align ourselves with the framed text
        PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
        PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
        PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.03f));
        PushStyleColor(ImGuiCol_PopupBg, ImVec4(ImColor(34, 34, 36)));
        bool ret = Begin(name, NULL, window_flags | ImGuiWindowFlags_NoScrollbar);
        PopStyleVar(3);
        PopStyleColor(2);
        if (!ret)
        {
            EndPopup();
            IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
            return false;
        }
        return true;
    }

    bool BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags)
    {
        using namespace ImGui;
        ImGuiContext& g = *GImGui;
        if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
        {
            g.NextWindowData.ClearFlags();
            return false;
        }

        // Set popup size
        float w = bb.GetWidth();
        if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
        {
            g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
        }
        else
        {
            if ((flags & ImGuiComboFlags_HeightMask_) == 0)
                flags |= ImGuiComboFlags_HeightRegular;
            IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
            int popup_max_height_in_items = -1;
            if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
            else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
            else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
            SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
        }

        // This is essentially a specialized version of BeginPopupEx()
        char name[16];
        ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

        // Set position given a custom constraint (peak into expected window size so we can position it)
        // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
        // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
        if (ImGuiWindow* popup_window = FindWindowByName(name))
            if (popup_window->WasActive)
            {
                // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
                ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
                popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
                ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
                ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, ImGuiPopupPositionPolicy_ComboBox);
                SetNextWindowPos(pos);
            }

        // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(g.Style.FramePadding.x, g.Style.WindowPadding.y)); // Horizontally align ourselves with the framed text
        bool ret = Begin(name, NULL, window_flags);
        PopStyleVar();
        if (!ret)
        {
            EndPopup();
            IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
            return false;
        }
        return true;
    }

    void EndCombo()
    {
        ImGui::EndPopup();
    }

    // Call directly after the BeginCombo/EndCombo block. The preview is designed to only host non-interactive elements
    // (Experimental, see GitHub issues: #1658, #4168)
    bool BeginComboPreview()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

        if (window->SkipItems || !window->ClipRect.Overlaps(g.LastItemData.Rect)) // FIXME: Because we don't have a ImGuiItemStatusFlags_Visible flag to test last ItemAdd() result
            return false;
        IM_ASSERT(g.LastItemData.Rect.Min.x == preview_data->PreviewRect.Min.x && g.LastItemData.Rect.Min.y == preview_data->PreviewRect.Min.y); // Didn't call after BeginCombo/EndCombo block or forgot to pass ImGuiComboFlags_CustomPreview flag?
        if (!window->ClipRect.Contains(preview_data->PreviewRect)) // Narrower test (optional)
            return false;

        // FIXME: This could be contained in a PushWorkRect() api
        preview_data->BackupCursorPos = window->DC.CursorPos;
        preview_data->BackupCursorMaxPos = window->DC.CursorMaxPos;
        preview_data->BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
        preview_data->BackupPrevLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
        preview_data->BackupLayout = window->DC.LayoutType;
        window->DC.CursorPos = preview_data->PreviewRect.Min + g.Style.FramePadding;
        window->DC.CursorMaxPos = window->DC.CursorPos;
        window->DC.LayoutType = ImGuiLayoutType_Horizontal;
        ImGui::PushClipRect(preview_data->PreviewRect.Min, preview_data->PreviewRect.Max, true);

        return true;
    }

    void EndComboPreview()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        ImGuiComboPreviewData* preview_data = &g.ComboPreviewData;

        // FIXME: Using CursorMaxPos approximation instead of correct AABB which we will store in ImDrawCmd in the future
        ImDrawList* draw_list = window->DrawList;
        if (window->DC.CursorMaxPos.x < preview_data->PreviewRect.Max.x && window->DC.CursorMaxPos.y < preview_data->PreviewRect.Max.y)
            if (draw_list->CmdBuffer.Size > 1) // Unlikely case that the PushClipRect() didn't create a command
            {
                draw_list->_CmdHeader.ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ClipRect = draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 2].ClipRect;
                draw_list->_TryMergeDrawCmds();
            }
        ImGui::PopClipRect();
        window->DC.CursorPos = preview_data->BackupCursorPos;
        window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, preview_data->BackupCursorMaxPos);
        window->DC.CursorPosPrevLine = preview_data->BackupCursorPosPrevLine;
        window->DC.PrevLineTextBaseOffset = preview_data->BackupPrevLineTextBaseOffset;
        window->DC.LayoutType = preview_data->BackupLayout;
        preview_data->PreviewRect = ImRect();
    }

    // Getter for the old Combo() API: const char*[]
    static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
    {
        const char* const* items = (const char* const*)data;
        if (out_text)
            *out_text = items[idx];
        return true;
    }

    // Getter for the old Combo() API: "item1\0item2\0item3\0"
    static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
    {
        // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
        const char* items_separated_by_zeros = (const char*)data;
        int items_count = 0;
        const char* p = items_separated_by_zeros;
        while (*p)
        {
            if (idx == items_count)
                break;
            p += strlen(p) + 1;
            items_count++;
        }
        if (!*p)
            return false;
        if (out_text)
            *out_text = p;
        return true;
    }

    // Old API, prefer using BeginCombo() nowadays if you can.
    bool Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
    {
        using namespace ImGui;
        ImGuiContext& g = *GImGui;

        // Call the getter to obtain the preview string which is a parameter to BeginCombo()
        const char* preview_value = NULL;
        if (*current_item >= 0 && *current_item < items_count)
            items_getter(data, *current_item, &preview_value);

        // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
        if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
            SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

        if (!Test::BeginCombo(label, preview_value, ImGuiComboFlags_None))
            return false;

        // Display items
        // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
        bool value_changed = false;
        for (int i = 0; i < items_count; i++)
        {
            PushID(i);
            const bool item_selected = (i == *current_item);
            const char* item_text;
            if (!items_getter(data, i, &item_text))
                item_text = "*Unknown item*";
            if (Test::Selectable(item_text, item_selected))
            {
                value_changed = true;
                *current_item = i;
            }
            if (item_selected)
                SetItemDefaultFocus();
            PopID();
        }

        EndCombo();

        if (value_changed)
            MarkItemEdited(g.LastItemData.ID);

        return value_changed;
    }

    // Combo box helper allowing to pass an array of strings.
    bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = 0)
    {
        const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
        return value_changed;
    }

    // Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
    bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items = 0)
    {
        int items_count = 0;
        const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
        while (*p)
        {
            p += strlen(p) + 1;
            items_count++;
        }
        bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
        return value_changed;
    }

}

using namespace ImGui;

const char* comboItems[] = { "Head", "Stomach", "Chest" };
int selectedItemIndex = 1;  // Index of the currently selected item

static int tab = 0;

namespace Menu
{
    char text_buffer[256] = "";
    bool authenticated = false, addmoney;
    float amount;
    ImVec4 color = ImVec4(1, 1, 1, 1);
    void DrawMenu()
    {
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {
            Begin(OBFUSCATE("Mystik Mods - Critical Ops v1.39.0.f2266"), &cops::menuOpen,
                  ImGuiWindowFlags_NoScrollbar);
            {
                static float original_y = ImGui::GetCursorPos().y;
                if (ImGui::BeginChild("Sidebar", ImVec2(200.f, ImGui::GetWindowHeight()))) {
                    if (Test::SideBtn("ESP", ImVec2(ImGui::GetWindowWidth(), 80.f))) tab = 0;
                    if (Test::SideBtn("CHAMS", ImVec2(ImGui::GetWindowWidth(), 80.f))) tab = 1;
                    if (Test::SideBtn("AIMBOT", ImVec2(ImGui::GetWindowWidth(), 80.f))) tab = 2;
                    if (Test::SideBtn("VISUALS", ImVec2(ImGui::GetWindowWidth(), 80.f))) tab = 3;
                    if (Test::SideBtn("WEAPON", ImVec2(ImGui::GetWindowWidth(), 80.f))) tab = 4;
                    if (Test::SideBtn("MISC", ImVec2(ImGui::GetWindowWidth(), 80.f))) tab = 5;

                }
                ImGui::EndChild();
                ImGui::SetCursorPos(ImVec2(310.f, original_y));
                if (ImGui::BeginChild("DataContent", ImVec2(0.f, 0.f))) {
                    switch (tab) {
                        case 0:
                            Test::Checkbox(OBFUSCATE("Enable ESP"), &cops::configs::esp::enabled);
                            Test::Checkbox(OBFUSCATE("User Names"), &cops::configs::esp::usernames);
                            Test::Checkbox(OBFUSCATE("Snaplines (TOP)"),
                                           &cops::configs::esp::linesTop);
                            Test::Checkbox(OBFUSCATE("Snaplines (BOTTOM)"),
                                           &cops::configs::esp::linesBottom);
                            Test::Checkbox(OBFUSCATE("2D Boxes"), &cops::configs::esp::boxes);
                            Test::Checkbox(OBFUSCATE("Outlined 2D Boxes"),
                                           &cops::configs::esp::outlined_boxes);
                            Test::Checkbox(OBFUSCATE("Filled 2D Boxes"),
                                           &cops::configs::esp::filled_boxes);
                            Test::Checkbox(OBFUSCATE("Health Bar"), &cops::configs::esp::healthbar);
                            Test::ColorPicker(OBFUSCATE("Color"), cops::configs::esp::color);
                            break;
                        case 1:
                            PushID(69);
                            Test::Checkbox(OBFUSCATE("Chams"), &cops::configs::chams::enabled);
                            PopID();
                            Test::Checkbox(OBFUSCATE("Shading"), &cops::configs::chams::shading);
                            Test::Checkbox(OBFUSCATE("Wireframe"),
                                           &cops::configs::chams::wireframe);
                            Test::Checkbox(OBFUSCATE("Outline"), &cops::configs::chams::outline);
                            Test::Checkbox(OBFUSCATE("Glow"), &cops::configs::chams::glow);
                            Test::Checkbox(OBFUSCATE("Rainbow"), &cops::configs::chams::rainbow);
                            PushID(420);
                            Test::ColorPicker(OBFUSCATE("Color"), cops::configs::chams::color);
                            PopID();
                            Test::SliderFloat(OBFUSCATE("Width"), &cops::configs::chams::width, 0.f,
                                              10.f);
                            break;
                        case 2:
                            Test::Checkbox(OBFUSCATE("Enable Aimbot"),
                                           &cops::configs::aimbot::enabled);
                            Test::Checkbox(OBFUSCATE("Silent Aimbot"),
                                           &cops::configs::aimbot::silent);
                            Test::Checkbox(OBFUSCATE("FOV Check"),
                                           &cops::configs::aimbot::fovCheck);
                            Test::SliderFloat(OBFUSCATE("FOV Value"),
                                              &cops::configs::aimbot::fovValue, 0.f, 360.f);
                            Test::Checkbox(OBFUSCATE("FOV Circle"),
                                           &cops::configs::aimbot::fovCircle);

                            // Inside your main loop or ImGui rendering function
                            Test::Combo(OBFUSCATE("Body Part"), &selectedItemIndex, comboItems,
                                        IM_ARRAYSIZE(comboItems));

                            // You can use the selectedItemIndex variable to determine which item is selected
                            switch (selectedItemIndex) {
                                case 0:
                                    cops::configs::aimbot::aimBone = 10;
                                    break;
                                case 1:
                                    cops::configs::aimbot::aimBone = 4;
                                    break;
                                case 2:
                                    cops::configs::aimbot::aimBone = 5;
                                    break;
                                default:
                                    break;
                            }
                            Test::Checkbox(OBFUSCATE("Red Dot"), &cops::configs::aimbot::redDot);
                            Test::Checkbox(OBFUSCATE("Visible Only"),
                                           &cops::configs::aimbot::visCheck);
                            Test::Checkbox(OBFUSCATE("On Shoot"), &cops::configs::aimbot::onShoot);
                            Test::Checkbox(OBFUSCATE("Enable Smoothness"),
                                           &cops::configs::aimbot::aimSmooth);
                            Test::SliderFloat(OBFUSCATE("Smoothness"),
                                              &cops::configs::aimbot::smoothAmount, 1.f,
                                              100.f);
                            break;
                        case 3:
                            Test::Checkbox(OBFUSCATE("Spectator Radar"), &cops::configs::visuals::specradar);
                            Test::Checkbox(OBFUSCATE("Radar"), &cops::configs::visuals::radar);
                            Test::Checkbox(OBFUSCATE("Team Radar"),
                                           &cops::configs::visuals::teamRadar);
                            Test::Checkbox(OBFUSCATE("Wide Putin"),
                                           &cops::configs::visuals::widePutin);
                            Test::SliderFloat(OBFUSCATE("Putin X"), &cops::configs::visuals::putinX,
                                              0.f, 20.f);
                            Test::SliderFloat(OBFUSCATE("Putin Y"), &cops::configs::visuals::putinY,
                                              0.f, 20.f);
                            Test::SliderFloat(OBFUSCATE("Putin Z"), &cops::configs::visuals::putinZ,
                                              0.f, 20.f);
                            Test::Checkbox(OBFUSCATE("No Flash"), &cops::configs::visuals::noFlash);
                            Test::Checkbox(OBFUSCATE("No Smoke"), &cops::configs::visuals::noSmoke);
                            Test::Checkbox(OBFUSCATE("No Sniper Blur"),
                                           &cops::configs::visuals::noSniperBlur);
                            Test::Checkbox(OBFUSCATE("View Model Changer"),
                                           &cops::configs::visuals::viewModel);
                            Test::SliderFloat(OBFUSCATE("View Model X"),
                                              &cops::configs::visuals::viewModelX,
                                              -1.f, 1.f);
                            Test::SliderFloat(OBFUSCATE("View Model Y"),
                                              &cops::configs::visuals::viewModelY,
                                              -1.f, 1.f);
                            Test::SliderFloat(OBFUSCATE("View Model Z"),
                                              &cops::configs::visuals::viewModelZ,
                                              -1.f, 1.f);
                            break;
                        case 4:
                            Test::Checkbox(OBFUSCATE("Always Crosshair"),
                                           &cops::configs::weapon::alwaysCrosshair);
                            Test::Checkbox(OBFUSCATE("No Spread"),
                                           &cops::configs::weapon::noSpread);
                            Test::Checkbox(OBFUSCATE("No Recoil"),
                                           &cops::configs::weapon::noRecoil);
                            Test::Checkbox(OBFUSCATE("Wall Shoot"),
                                           &cops::configs::weapon::wallShot);
                            Test::Checkbox(OBFUSCATE("Fire Rate"),
                                           &cops::configs::weapon::fireRateActive);
                            Test::SliderFloat(OBFUSCATE("Fire Rate Modifier"),
                                              &cops::configs::weapon::fireRate,
                                              0.f,
                                              10.f);
                            Test::Checkbox(OBFUSCATE("Head Rain"),
                                           &cops::configs::weapon::headRain);
                            Test::Checkbox(OBFUSCATE("No Aimpunch"),
                                           &cops::configs::weapon::noAimpunch);
                            Test::Checkbox(OBFUSCATE("No Dryfire"),
                                           &cops::configs::weapon::noDryfire);
                            Test::Checkbox(OBFUSCATE("Magic Melee"),
                                           &cops::configs::weapon::magicMelee);
                            Test::Checkbox(OBFUSCATE("Instant Equip Gun"),
                                           &cops::configs::weapon::instantEquipGun);
                            break;
                        case 5:
                            Test::Checkbox(OBFUSCATE("Speed"),
                                           &cops::configs::miscellaneous::speed);
                            Test::Checkbox(OBFUSCATE("No Clip"),
                                           &cops::configs::miscellaneous::noclip);
                            Test::SliderFloat(OBFUSCATE("Speed Value"),
                                              &cops::configs::miscellaneous::speedValue, 0.f,
                                              30.f);
                            Test::Checkbox(OBFUSCATE("Bomb Timer"),
                                           &cops::configs::miscellaneous::bombTimer);
                            Test::Checkbox(OBFUSCATE("Jump Height"),
                                           &cops::configs::miscellaneous::jumpHeight);
                            Test::SliderFloat(OBFUSCATE("Jump Height Value"),
                                              &cops::configs::miscellaneous::jumpVal,
                                              0.f, 30.f);
                            Test::Checkbox(OBFUSCATE("Bots Don't Shoot"),
                                           &cops::configs::miscellaneous::botsNoShoot);
                            Test::Checkbox(OBFUSCATE("Fly"), &cops::configs::miscellaneous::fly);
                            Test::SliderFloat(OBFUSCATE("Fly Height"),
                                              &cops::configs::miscellaneous::flyVal, 1.47f,
                                              10.f);
                            Test::Checkbox(OBFUSCATE("Skip Tutorial"),
                                           &cops::configs::miscellaneous::skipTut);
                            break;
                        case 6:
                            break;
                        default:
                            break;
                    }

                }
                ImGui::EndChild();
            }
            End();
        }
    }

}
#endif MENU