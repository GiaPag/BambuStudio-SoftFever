#ifndef slic3r_GUI_AmsMappingPopup_hpp_
#define slic3r_GUI_AmsMappingPopup_hpp_

#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/collpane.h>
#include <wx/dataview.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/dataview.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/popupwin.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include <wx/wrapsizer.h>

#include "GUI_Utils.hpp"
#include "wxExtensions.hpp"
#include "DeviceManager.hpp"
#include "Plater.hpp"
#include "BBLStatusBar.hpp"
#include "BBLStatusBarSend.hpp"
#include "Widgets/Label.hpp"
#include "Widgets/Button.hpp"
#include "Widgets/CheckBox.hpp"
#include "Widgets/ComboBox.hpp"
#include "Widgets/ScrolledWindow.hpp"
#include "Widgets/PopupWindow.hpp"
#include <wx/simplebook.h>
#include <wx/hashmap.h>

namespace Slic3r { namespace GUI {

#define MATERIAL_ITEM_SIZE wxSize(FromDIP(64), FromDIP(34))
#define MATERIAL_ITEM_REAL_SIZE wxSize(FromDIP(62), FromDIP(32))
#define MAPPING_ITEM_REAL_SIZE wxSize(FromDIP(48), FromDIP(45))
#define AMS_TOTAL_COUNT 4

enum TrayType {
    NORMAL,
    THIRD,
    EMPTY
};

struct TrayData
{
    TrayType        type;
    int             id;
    std::string     name;
    std::string     filament_type;
    wxColour        colour;
};

class MaterialItem: public wxPanel
{
public:
    MaterialItem(wxWindow *parent,wxColour mcolour, wxString mname);
    ~MaterialItem();

    wxColour    m_material_coloul;
    wxString    m_material_name;

    wxColour m_ams_coloul;
    wxString m_ams_name;

    ScalableBitmap m_arraw_bitmap_gray;
    ScalableBitmap m_arraw_bitmap_white;

    bool m_selected {false};
    bool m_warning{false};

    void msw_rescale();
    void set_ams_info(wxColour col, wxString txt);

    void on_normal();
    void on_selected();
    void on_warning();

    void on_left_down(wxMouseEvent &evt);
    void paintEvent(wxPaintEvent &evt);
    void render(wxDC &dc);
    void doRender(wxDC &dc);
};

class MappingItem : public wxPanel
{
public:
    MappingItem(wxWindow *parent);
    ~MappingItem();

	void update_data(TrayData data);
    void send_event(int fliament_id);
    void set_tray_index(wxString t_index) {m_tray_index = t_index;};

    wxString m_tray_index;
    wxColour m_coloul;
    wxString m_name;
    TrayData m_tray_data;
    bool     m_unmatch{false};

    void msw_rescale();
    void paintEvent(wxPaintEvent &evt);
    void render(wxDC &dc);
    void set_data(wxColour colour, wxString name, TrayData data, bool unmatch = false);
    void doRender(wxDC &dc);
};

class MappingContainer : public wxPanel
{
public:
    wxBitmap  ams_mapping_item_container;
    MappingContainer(wxWindow* parent);
    ~MappingContainer();
    void paintEvent(wxPaintEvent& evt);
    void render(wxDC& dc);
    void doRender(wxDC& dc);
};

class AmsMapingPopup : public PopupWindow
{
public:
    AmsMapingPopup(wxWindow *parent);
    wxString format_text(wxString &m_msg);
    ~AmsMapingPopup(){};

    wxStaticText *           m_warning_text{nullptr}; 
    std::vector<std::string> m_materials_list;
    std::vector<wxBoxSizer*> m_amsmapping_container_sizer_list;
    std::vector<wxWindow*>   m_amsmapping_container_list;
    std::vector<MappingItem*> m_mapping_item_list;

    bool        m_has_unmatch_filament {false};
    int         m_current_filament_id;
    std::string m_tag_material;
    wxBoxSizer *m_sizer_main{nullptr}; 
    wxBoxSizer *m_sizer_list{nullptr}; 

    void         update_materials_list(std::vector<std::string> list);
    void         set_tag_texture(std::string texture);
    void         update_ams_data(std::map<std::string, Ams *> amsList);
    void         add_ams_mapping(std::vector<TrayData> tray_data, wxWindow* container, wxBoxSizer* sizer);
    void         set_current_filament_id(int id){m_current_filament_id = id;};
    int          get_current_filament_id(){return m_current_filament_id;};
    bool         is_match_material(std::string material);
    void         on_left_down(wxMouseEvent &evt);
    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown(wxMouseEvent &event) wxOVERRIDE;
    void         paintEvent(wxPaintEvent &evt);
    std::vector<TrayData> parse_ams_mapping(std::map<std::string, Ams*> amsList);
};

class AmsMapingTipPopup : public PopupWindow
{
public:
    AmsMapingTipPopup(wxWindow *parent);
    ~AmsMapingTipPopup(){};
    void paintEvent(wxPaintEvent &evt);

    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown(wxMouseEvent &event) wxOVERRIDE;

public:
    wxPanel *        m_panel_enable_ams;
    wxStaticText *   m_title_enable_ams;
    wxStaticText *   m_tip_enable_ams;
    wxPanel *        m_split_lines;
    wxPanel *        m_panel_disable_ams;
    wxStaticText *   m_title_disable_ams;
    wxStaticText *   m_tip_disable_ams;
};

class AmsHumidityTipPopup : public PopupWindow
{
public:
    AmsHumidityTipPopup(wxWindow* parent);
    ~AmsHumidityTipPopup() {};
    void paintEvent(wxPaintEvent& evt);

    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown(wxMouseEvent& event) wxOVERRIDE;

public:
    wxStaticBitmap* m_img;
    Label* m_staticText1;
    Label* m_staticText2;
    Label* m_staticText3;
    Label* m_staticText4;
    Label* m_staticText_note;
    Button* m_button_confirm;
};

class AmsTutorialPopup : public PopupWindow
{
public:
    Label* text_title;
    wxStaticBitmap* img_top;
    wxStaticBitmap* arrows_top;
    wxStaticText* tip_top;
    wxStaticBitmap* arrows_bottom;
    wxStaticText* tip_bottom;
    wxStaticBitmap* img_middle;
    wxStaticText* tip_middle;
    wxStaticBitmap* img_botton;

    AmsTutorialPopup(wxWindow* parent);
    ~AmsTutorialPopup() {};

    void paintEvent(wxPaintEvent& evt);
    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown(wxMouseEvent& event) wxOVERRIDE;
};


class AmsIntroducePopup : public PopupWindow
{
public:
    bool          is_enable_ams = {false};
    Label* m_staticText_top;
    Label* m_staticText_bottom;
    wxStaticBitmap* m_img_enable_ams;
    wxStaticBitmap* m_img_disable_ams;

    AmsIntroducePopup(wxWindow* parent);
    ~AmsIntroducePopup() {};

    void set_mode(bool enable_ams);
    void paintEvent(wxPaintEvent& evt);
    virtual void OnDismiss() wxOVERRIDE;
    virtual bool ProcessLeftDown(wxMouseEvent& event) wxOVERRIDE;
};


wxDECLARE_EVENT(EVT_SET_FINISH_MAPPING, wxCommandEvent);

}} // namespace Slic3r::GUI

#endif
