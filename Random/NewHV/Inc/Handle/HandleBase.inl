namespace ssvu
{
	namespace Internal
	{
		template<typename T> inline bool Internal::HVHandleBase<T>::isAlive() const noexcept
		{
			return hVec->marks[markIdx].ctr == ctr;
		}

		template<typename T> inline void Internal::HVHandleBase<T>::setDead() noexcept
		{
			hVec->setDeadFromMark(markIdx);
		}
	}
}