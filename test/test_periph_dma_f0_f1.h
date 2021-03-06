BOOST_AUTO_TEST_SUITE (test_suite_main)

mcu::DMA dma;
auto& CMSIS = dma.like_CMSIS();

BOOST_AUTO_TEST_CASE (like_CMSIS)
{
   auto same = std::is_same_v<std::remove_reference_t<decltype(CMSIS)>, DMA_TypeDef>;
   auto address_rcc = reinterpret_cast<size_t>(&dma);
   auto address_CMSIS = reinterpret_cast<size_t>(&CMSIS);
   BOOST_CHECK_EQUAL (address_rcc, address_CMSIS);
   BOOST_CHECK_EQUAL (same, true);
}

BOOST_AUTO_TEST_CASE (clear_interrupt_flags)
{
   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_1);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF1_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_2);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF2_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_3);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF3_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_4);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF4_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_5);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF5_Msk);
   
#if defined(STM32F1)
   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_6);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF6_Msk);

   CMSIS.IFCR = 0;
   dma.clear_interrupt_flags(mcu::DMA::Channel::_7);
   BOOST_CHECK_EQUAL (CMSIS.IFCR, DMA_IFCR_CGIF7_Msk);
#endif
}

BOOST_AUTO_TEST_CASE(is_transfer_complete_interrupt)
{
   CMSIS.ISR = DMA_ISR_TCIF1_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_1), true);

   CMSIS.ISR = DMA_ISR_TCIF2_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_2), true);

   CMSIS.ISR = DMA_ISR_TCIF3_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_3), true);

   CMSIS.ISR = DMA_ISR_TCIF4_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_4), true);

   CMSIS.ISR = DMA_ISR_TCIF5_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_5), true);

#if defined(STM32F1)
   CMSIS.ISR = DMA_ISR_TCIF6_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_6), true);

   CMSIS.ISR = DMA_ISR_TCIF7_Msk;
   BOOST_CHECK_EQUAL (dma.is_transfer_complete_interrupt(mcu::DMA::Channel::_7), true);
#endif
}


BOOST_AUTO_TEST_SUITE_END()

