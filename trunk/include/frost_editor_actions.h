namespace Frost
{
    class ActionAddDoodad : public EditorAction
    {
    public :

        ActionAddDoodad(const s_str& sName, const s_str& sModel) :
            sName_(sName), sModel_(sModel)
        {
        }

        void Do()
        {
            s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
            if (!pZone)
                throw Exception("No Zone loaded, can't create doodad.");

            s_ptr<Doodad> pDoodad = pZone->AddDoodad(sName_, sModel_);

            if (pDoodad)
                SceneManager::GetSingleton()->StartMoving(pDoodad, Vector::CONSTRAINT_NONE, true);

            EventManager::GetSingleton()->FireEvent(Event("DOODAD_ADDED"));
            EventManager::GetSingleton()->FireEvent(Event("ZONE_MODIFIED"));
        }

        void Undo()
        {
            // TODO : # Implement AddDoodad::Undo.
        }

    private :

        s_str sName_;
        s_str sModel_;
    };
}
