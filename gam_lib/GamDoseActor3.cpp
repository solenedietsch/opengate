/* --------------------------------------------------
   Copyright (C): OpenGATE Collaboration
   This software is distributed under the terms
   of the GNU Lesser General  Public Licence (LGPL)
   See LICENSE.md for further details
   -------------------------------------------------- */

#include "GamDoseActor3.h"
#include "G4RandomTools.hh"
#include "G4SystemOfUnits.hh"
#include "itkImageFileWriter.h"

GamDoseActor3::GamDoseActor3() : GamVActor("DoseActor3") {
    // Create the image pointer
    // size and allocation will be performed on the py side
    cpp_image = itk::Image<float, 3>::New();
}

void GamDoseActor3::BeforeStart() {
    // std::cout << "DoseActor3 Before Start" << std::endl;
}

void GamDoseActor3::SaveImage() {
    std::cout << "DEBUG save img" << std::endl;
    using WriterType = itk::ImageFileWriter<GamDoseActor3::ImageType>;
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("cpp_dose.mhd");
    writer->SetInput(cpp_image);
    writer->Update();
}


G4bool GamDoseActor3::ProcessHits(G4Step *step, G4TouchableHistory *touchable) {
    //  std::cout << "GamDoseActor3 cpp ProcessHits " << " " << batch_step_count << std::endl;
    /*
     Every step (or hit), the position and edep are stored in a vector.
     Every 'batch_size' step, the SteppingBatchAction function is called (will be overloaded in gam python side)
     */
    SteppingAction(step, touchable); // FIXME not really needed
    return true;
}

void GamDoseActor3::SteppingAction(G4Step *step, G4TouchableHistory *) {
    auto preGlobal = step->GetPreStepPoint()->GetPosition();
    auto postGlobal = step->GetPostStepPoint()->GetPosition();
    auto touchable = step->GetPreStepPoint()->GetTouchable();
    auto depth = 0;//touchable->GetHistoryDepth();
    // Depth = 0 bottom level
    // Depth = 1 = mother
    // Depth = 2 grand mother

    // random position along the step
    auto x = G4UniformRand();
    auto direction = postGlobal - preGlobal;
    auto position = preGlobal + x * direction;
    auto localPosition = touchable->GetHistory()->GetTransform(depth).TransformPoint(position);

    // convert G4ThreeVector to itk PointType
    point[0] = localPosition[0];
    point[1] = localPosition[1];
    point[2] = localPosition[2];

    // set image pixel
    // FIXME hit middle/random etc
    auto edep = step->GetTotalEnergyDeposit() / MeV;
    cpp_image->TransformPhysicalPointToIndex(point, index);
    /*std::cout << "depth=" << depth
              << " x " << x << std::endl
              << "tr0 " << touchable->GetHistory()->GetTransform(0).NetTranslation()
              << "tr1 " << touchable->GetHistory()->GetTransform(1).NetTranslation()
              << "tr2 " << touchable->GetHistory()->GetTransform(2).NetTranslation()
              << std::endl
              << " spac " << cpp_image->GetSpacing()
              << " pG " << preGlobal
              << " p " << localPosition
              << " -> " << index << std::endl;
              */
    if (cpp_image->GetLargestPossibleRegion().IsInside(index)) {
        edep += cpp_image->GetPixel(index);
        cpp_image->SetPixel(index, edep);
    } else {
        //std::cout << "outside" << std::endl;
    }
}
